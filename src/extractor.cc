#include <sstream>
#include "extractor.h"

using std::stringstream;

Extractor::Extractor()
{
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

void Extractor::handler(const Mat &frame, string cam_id,
                        string video_id, size_t frame_pos)
{
    // set frame reference
    if (!is_init_) {
        set_frame_refer(frame);
    }

    if (HistDiff(frame_refer_, frame)) {
        // found human and get bound in rectangle
        vector<Rect> found_rects(HumanDetect(frame));

		for (int i = 0; i < found_rects.size(); i++) {
			
			Mat person_image;
			frame(found_rects[i]).copyTo(person_image);

			imshow("Extractor Debug", person_image);
			//get feature of each person image
			Mat person_feature=get_feature_.getFeature(person_image);

			//get personshot
			string id = get_id(cam_id, video_id, frame_pos, i);

			vector<int> rect;
			rect.push_back(found_rects[i].x);
			rect.push_back(found_rects[i].y);
			rect.push_back(found_rects[i].x + found_rects[i].width);
			rect.push_back(found_rects[i].y + found_rects[i].height);

			cout << id << endl;
			cout << rect[0] << " " << rect[1] << " " << rect[2] << " " << rect[3] << endl;
			cout << person_feature << endl;

			PersonShot person_shot_(id, cam_id, video_id, frame_pos, rect, person_feature);

			// TODO:save personshot
		}

        // update frame refer
        set_frame_refer(frame);
    }
}

// Form id.
//
string Extractor::get_id(const string cam_id, const string video_id,
                         const size_t frame_pos, int sequence) {
	stringstream ss, sss;
	string str_frame_pos, str_sequence;
	ss << frame_pos;
	ss >> str_frame_pos;
	if (str_frame_pos.size() == 1) {
		str_frame_pos = "0000" + str_frame_pos;
	}
	if (str_frame_pos.size() == 2) {
		str_frame_pos = "000" + str_frame_pos;
	}
	if (str_frame_pos.size() == 3) {
		str_frame_pos = "00" + str_frame_pos;
	}
	if (str_frame_pos.size() == 4) {
		str_frame_pos = "0" + str_frame_pos;
	}
	sss << sequence;
	sss >> str_sequence;
	if (str_sequence.size() == 1) {
		str_sequence = "0" + str_sequence;
	}

	return cam_id + video_id + str_frame_pos + str_sequence;
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
    const float kDiffThreshold = 0.95;

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
    calcHist(&hsv_f_t1, 1, channels, Mat(), hist_f_t1, 2, hist_size, ranges, true, false);
    normalize(hist_f_t1, hist_f_t1, 0, 1, NORM_MINMAX, -1, Mat());

    calcHist(&hsv_f_t2, 1, channels, Mat(), hist_f_t2, 2, hist_size, ranges, true, false);
    normalize(hist_f_t2, hist_f_t2, 0, 1, NORM_MINMAX, -1, Mat());

    // $start test
    // apply the histogram comparison methods
    /*
    for (int i = 0; i < 4; i++ )
    {
        int compare_method = i;
        double f1_f2 = compareHist(hist_f_t1, hist_f_t2, compare_method);
        fprintf(stdout, " Method [%d] f1-f2 : %f \n", i, f1_f2);
    }
    */
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
    // Prepare HOG descripter
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    hog.detectMultiScale(frame, found_rects, 0, Size(8,8), Size(32, 32), 1.05, 2);
    // filter?
    for (int i = 0; i < found_rects.size(); i++) {
        Rect r = found_rects[i];
        int j = 0;
        for (j = 0; j < found_rects.size(); j++)
            if (j != i && (r & found_rects[j]) == r) break;
        if (j == found_rects.size())
            found_rects_filtered.push_back(r);
    }

    return found_rects_filtered;
}
