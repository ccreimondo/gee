#include "extractor.h"
#include "memcache.h"
#include "sugar/sugar.h"
#include "sugar/gdebug.h"

using namespace cv;
using std::string;

Extractor::Extractor()
{
    path_ = "/tmp/gee/keyframes/";
    filename_ = "";
    is_init_ = false;
}

void Extractor::set_frame_refer(const Mat &frame)
{
    // init Mat with the frame from video stream
    if (frame_refer_.empty()) {
        frame_refer_ = frame.clone();
    // TODO (@Zhiqiang He): reset Mat's content
    } else {
        frame_refer_.release();
        frame_refer_ = frame.clone();
    }

    is_init_ = true;
}

void Extractor::handler(const IPCamera ip_camera, const string &video_id,
                        const size_t frame_pos, const Mat &frame)
{
    // set frame reference
    if (!is_init_) {
        set_frame_refer(frame);
    }

    // extracting keyframe
    if (HistDiff(frame_refer_, frame)) {
#ifndef NOGDEBUG
        cout << "New keyframe!" << endl;
#endif
        // cache the new keyframe
        filename_ = ip_camera.get_id() +
                    video_id + FormatUnsignedInt(frame_pos, 5) + ".jpeg";
        string fullpath = path_ + filename_;
        vector<int> c_params;
        c_params.push_back(CV_IMWRITE_JPEG_QUALITY);
        imwrite(fullpath, frame, c_params);
        KeyframeShot key_frame_shot(video_id, ip_camera.get_id(),
                                    frame_pos,
                                    path_, filename_,
                                    frame);
        memcache_.save(key_frame_shot);

        // found human and get bound in rectangle
        vector<Rect> found_rects(HumanDetect(frame));

        for (size_t i = 0; i < found_rects.size(); i++) {
#ifndef NOGDEBUG
            // Mat frame_show = frame.clone();
            // rectangle(frame_show, found_rects[i], Scalar(0 ,255, 255));
            // imshow("rect", frame_show);
#endif

            Mat person_image;

            try {
                person_image = frame(found_rects[i]).clone();
            } catch (const char *e) {
                LogError(e);
            }

            resize(person_image, person_image, Size(48, 128), 0, 0, INTER_AREA);

#ifndef NOGDEBUG
                imshow("person shot", person_image);
#endif

            // get feature of each person image
            Mat person_feature = get_feature_.getFeature(person_image);

			vector<int> rect;
			rect.push_back(found_rects[i].x);
			rect.push_back(found_rects[i].y);
			rect.push_back(found_rects[i].x + found_rects[i].width);
			rect.push_back(found_rects[i].y + found_rects[i].height);

            PersonShot person_shot(i,
                                   ip_camera.get_id(),
                                   video_id,
                                   key_frame_shot.get_id(),
                                   frame_pos,
                                   rect,
                                   person_feature);
            memcache_.save(person_shot);
#ifndef NOGDEBUG
        cout << "Proper vector save done!" << endl;
#endif
        }

        // update frame refer
        set_frame_refer(frame);
    }
}

// Judge keyframe by diff frame.
//
bool FrameDiff(const Mat &frame_t1, const Mat &frame_t2)
{
    Mat _frame_t1, _frame_t2, abs_ans;

    cvtColor(frame_t1, _frame_t1, COLOR_RGB2GRAY);
    cvtColor(frame_t2, _frame_t2, COLOR_RGB2GRAY);

    absdiff(_frame_t1, _frame_t2, abs_ans);
    // TODO (@Zhiqiang He): set threshold

    return false;
}

// Judge keyframe by diff histogram of frames.
//
bool HistDiff(const Mat &frame_t1, const Mat &frame_t2)
{
    const float kDiffThreshold = 0.98;

    Mat hsv_f_t1, hsv_f_t2;

    // convert to HSV
    cvtColor(frame_t1, hsv_f_t1, COLOR_BGR2HSV);
    cvtColor(frame_t2, hsv_f_t2, COLOR_BGR2HSV);

    // using 50 bins for hue and 60 for saturation
    int h_bins = 50; int s_bins = 60;
    int hist_size[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float *ranges[] = { h_ranges, s_ranges };

    // use the o-th and 1-st channels
    int channels[] = { 0, 1 };

    // histograms
    MatND hist_f_t1;
    MatND hist_f_t2;

    // calculate the histograms for the HSV images
    calcHist(&hsv_f_t1, 1, channels, Mat(), hist_f_t1,
             2, hist_size, ranges, true, false);
    normalize(hist_f_t1, hist_f_t1, 0, 1, NORM_MINMAX, -1, Mat());

    calcHist(&hsv_f_t2, 1, channels, Mat(), hist_f_t2,
             2, hist_size, ranges, true, false);
    normalize(hist_f_t2, hist_f_t2, 0, 1, NORM_MINMAX, -1, Mat());

    // $start test
    // apply the histogram comparison methods
    // for (int i = 0; i < 4; i++ )
    // {
    //     int compare_method = i;
    //     double f1_f2 = compareHist(hist_f_t1, hist_f_t2, compare_method);
    //     fprintf(stdout, " Method [%d] f1-f2 : %f \n", i, f1_f2);
    // }
    // $end test

    double f1_f2= compareHist(hist_f_t1, hist_f_t2, CV_COMP_CORREL);
    if (f1_f2 >= kDiffThreshold) return false;

    return true;
}

// Human detect.
//
vector<Rect> HumanDetect(const Mat &frame)
{
    vector<Rect> found_rects, found_rects_filtered;
    // prepare HOG descripter
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    hog.detectMultiScale(frame, found_rects, 0, Size(8,8), Size(0, 0), 1.05, 2);

    for (size_t i = 0; i < found_rects.size(); i++) {
        Rect r = found_rects[i];
        // to fix over bound bug
        // if (flag) continue;
        bool overbound = !(0 <= r.x && 0 <= r.width &&
                           r.x + r.width <= frame.cols &&
                           0 <= r.y && 0 <= r.height &&
                           r.y + r.height <= frame.rows);
        if (overbound) continue;

        size_t j = 0;
        for (j = 0; j < found_rects.size(); j++)
            if (j != i && (r & found_rects[j]) == r) break;
        if (j == found_rects.size())
            found_rects_filtered.push_back(r);
    }

    for (size_t i = 0; i<found_rects_filtered.size(); i++) {
        found_rects_filtered[i].x += cvRound(found_rects_filtered[i].width*0.2);
        found_rects_filtered[i].width = cvRound(found_rects_filtered[i].width*0.6);
        found_rects_filtered[i].y += cvRound(found_rects_filtered[i].height*0.07);
        found_rects_filtered[i].height = cvRound(found_rects_filtered[i].height*0.8);
    }

    return found_rects_filtered;
}
