#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include "sugar.h"

using namespace cv;
using std::string;
using std::cout;
using std::endl;

const char *kDirPrefix = "/home/reimondo/Workspace/Projects/gee/";
const int kCount = 10;
const int kBufferSize = 100;

void Dispatcher();
// $start debug
void Test();
// $end debug

int main()
{
    Dispatcher();
    // Test();

    exit(0);
}

void Test()
{
    cout << NumberToString<float>(1.444) << endl;
}

void Dispatcher()
{
    char video_stream_addr[kBufferSize];
    // sprintf(video_stream_addr, "%s%s", kDirPrefix, "cam.sdp");
    sprintf(video_stream_addr, "%s%s", kDirPrefix, "raw/300w.flv");

    // create video stream capture
    VideoCapture cap(video_stream_addr);
    if (!cap.isOpened()) {
        LogError("Fail to open video stream.");
        exit(1);
    }

    // define a window to display
    namedWindow("See see", WINDOW_AUTOSIZE);

    char video_saving_addr[kBufferSize];
    sprintf(video_saving_addr, "%s%s", kDirPrefix, "video/");
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

    Mat frame;
    while (cap.read(frame)) {
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


        // Stage 3 - motion detect and output.
        if (cap.read(frame)) {
            imshow("edges", frame);
        }
        if (waitKey(1) >= 0) break;
    }
}
