#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include "sugar.h"

using namespace cv;
using std::string;
using std::vector;
using std::cout;
using std::endl;

const char *kDirPrefix = "/media/reimondo/HDD/Workspace/Projects/gee/";
const int kCount = 10;
const int kBufferSize = 100;

void Dispatcher();
// $start debug
void Test();
// $end debug

int main()
{
    // Dispatcher();
    Test();

    exit(0);
}

// Judge keyframe by diff frame.
//
bool FrameDiff(Mat &frame_t1, Mat &frame_t2)
{
    Mat _frame_t1, _frame_t2, abs_ans;

    cvtColor(frame_t1, _frame_t1, COLOR_RGB2GRAY);
    cvtColor(frame_t2, _frame_t2, COLOR_RGB2GRAY);

    absdiff(_frame_t1, _frame_t2, abs_ans);
    cout << abs_ans << endl;

    return false;
}

// Judge keyframe by diff histogram of frames.
//
bool HistDiff(Mat &frame_t1, Mat &frame_t2)
{
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

    const float* ranges[] = { h_ranges, s_ranges };

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
    for( int i = 0; i < 4; i++ )
    {
        int compare_method = i;
        double f1_f2 = compareHist(hist_f_t1, hist_f_t2, compare_method);
        fprintf(stdout, " Method [%d] f1-f2 : %f \n", i, f1_f2);
    }
    */
    // $end test

    double f1_f2= compareHist(hist_f_t1, hist_f_t2, CV_COMP_CORREL);
    if (f1_f2 >= 0.95) return false;

    return true;
}

// Dispatcher.
//
void Dispatcher()
{
    char video_stream_addr[kBufferSize];
    // sprintf(video_stream_addr, "%s%s", kDirPrefix, "cam.sdp");
    sprintf(video_stream_addr, "%s%s", kDirPrefix, "example/videos/300w.flv");

    // create video stream capture
    VideoCapture cap(video_stream_addr);
    if (!cap.isOpened()) {
        LogError("Fail to open video stream.");
        exit(1);
    }

    char video_saving_addr[kBufferSize];
    sprintf(video_saving_addr, "%s%s", kDirPrefix, "videos/");
    // create video stream writer
    VideoWriter writer;

    // define where to save pictures
    char pic_saving_addr[kBufferSize];
    sprintf(pic_saving_addr, "%s%s", kDirPrefix, "pictures/");

    // init timestamp
    double timestamp_before = cap.get(CV_CAP_PROP_POS_MSEC);
    double timestamp_after = cap.get(CV_CAP_PROP_POS_MSEC);

    // start the first writer
    char video_name_1st[kBufferSize];
    sprintf(video_name_1st, "%s%.0lf-%.0lf.%s",
            video_saving_addr, timestamp_after, timestamp_after+60000, "h264");

    Size framesize(cap.get(CV_CAP_PROP_FRAME_WIDTH),
                   cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    writer.open(video_name_1st, CV_FOURCC('X', '2', '6', '4'),
                cap.get(CV_CAP_PROP_FPS), framesize);
    if (!writer.isOpened()) {
        LogError("Fail to open the first saving file!");
        exit(0);
    }

    Mat frame, frame_before;
    while (cap.read(frame)) {
        // init frame_before and save the fisrt frame
        if (frame_before.empty()) {
            frame_before = frame.clone();

            char keyframe_name[kBufferSize];
            sprintf(keyframe_name, "%s%s%.0lf.%s",
                    pic_saving_addr, "kf_", cap.get(CV_CAP_PROP_POS_MSEC), "jpeg");
            vector<int> c_params;
            c_params.push_back(CV_IMWRITE_JPEG_QUALITY);
            imwrite(keyframe_name, frame, c_params);
        }

        // Stage 1 - save, 1min per piece.
        // update the second timestamp
        timestamp_after = cap.get(CV_CAP_PROP_POS_MSEC);
        if ((timestamp_after - timestamp_before) >= 60000) {    // 1min = 60000ms
            // release old video writer
            writer.release();
            // create new filename
            char video_name[kBufferSize];
            sprintf(video_name, "%s%.0lf-%.0lf.%s",
                    video_saving_addr, timestamp_after, timestamp_after+60000, "h264");

            Size framesize(cap.get(CV_CAP_PROP_FRAME_WIDTH),
                           cap.get(CV_CAP_PROP_FRAME_HEIGHT));
            // create a new video writer
            writer.open(video_name, CV_FOURCC('X', '2', '6', '4'),
                        cap.get(CV_CAP_PROP_FPS), framesize);
            if (!writer.isOpened()) {
                LogError("Fail to open saving file!");
                exit(0);
            }
            writer << frame;
            // update the first timestamp
            timestamp_before = cap.get(CV_CAP_PROP_POS_MSEC);
        } else {
            // continue to write
            writer << frame;
        }

        // Stage 2 - extract.
        if (HistDiff(frame_before, frame)) {
            char keyframe_name[kBufferSize];
            sprintf(keyframe_name, "%s%s%.0lf.%s",
                    pic_saving_addr, "kf_", cap.get(CV_CAP_PROP_POS_MSEC), "jpeg");
            vector<int> c_params;
            c_params.push_back(CV_IMWRITE_JPEG_QUALITY);
            imwrite(keyframe_name, frame, c_params);

            // update old frame
            frame_before.release();
            frame_before = frame.clone();
        }
        // show key frame change
        imshow("frame_before", frame_before);

        // Stage 3 - motion detect and output.

        imshow("frame", frame);
        if (waitKey(1) == 27) break;
    }
}

void Test()
{
    char example_addr[kBufferSize];
    sprintf(example_addr, "%s%s", kDirPrefix, "example/");

    char test_pics_addr[kBufferSize], test_videos_addr[kBufferSize];
    sprintf(test_pics_addr, "%s%s", kDirPrefix, "pictures/");
    sprintf(test_videos_addr, "%s%s", kDirPrefix, "videos/");

    char pic1[kBufferSize];
    char pic2[kBufferSize];
    sprintf(pic1, "%s%s", test_pics_addr, "0.jpg");
    sprintf(pic2, "%s%s", test_pics_addr, "9.jpg");
    Mat frame_t1 = imread(pic1);
    Mat frame_t2 = imread(pic2);
    imshow("-_-", frame_t1);
    imshow("-_-", frame_t2);
    FrameDiff(frame_t1, frame_t2);
}
