#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include "sugar/sugar.h"
#include "videostreamhandler.h"
#include "gdatatype.h"
#include "gdebug.h"

using namespace cv;
using std::string;
using std::vector;

const int kBufferSize = 80;
const string g_dir_prefix = "/media/reimondo/HDD/Workspace/Projects/gee/";

void Dispatcher();
void MotionDetect(Mat &frame);
void Test();

int main()
{
    Dispatcher();
    // Test();

    exit(0);
}

void Test()
{
#ifndef NOGDEBUG
    cout << IP2HexStr("192.168.3.1") << endl;
    cout << FormatUnsignedInt(10, 1) << endl;
#endif
}

// Dispatcher.
//
void Dispatcher()
{
    char video_stream_addr[kBufferSize];
    // sprintf(video_stream_addr, "%s%s", g_dir_prefix, "cam.sdp");
    sprintf(video_stream_addr, "%s%s",
            g_dir_prefix.c_str(),
            "example/videos/WP_20151002_09_40_51_Pro_lq.mp4");

    IPCamera ip_camera("192.168.3.1", "");
    VideoStreamHandler(video_stream_addr, ip_camera);
}
