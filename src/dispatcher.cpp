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
using std::to_string;

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
    const string kDirPrefix = "/media/reimondo/HDD/Workspace/Projects/gee/";
    string video_stream_addr;

    // video stream from raspberry pi
    // sprintf(video_stream_addr, "%s%s", g_dir_prefix, "cam.sdp");

    // static videos files for debugging
    vector<string> video_list;
    video_list.push_back("8_x264.mkv");
    video_list.push_back("38_720.mp4");
    video_list.push_back("218_x264_720.mkv");
    video_list.push_back("220_x264_720.mkv");
    for (size_t i = 0; i < video_list.size(); ++i) {
        video_stream_addr = kDirPrefix + "example/videos/" + video_list[i];
        string camera_ip("192.168.3.");
        camera_ip += to_string(i);
        IPCamera ip_camera(camera_ip, "cqu");
        try {
            VideoStreamHandler(video_stream_addr, ip_camera);
        } catch (const char *e) {
            LogInfo("Exception", "something done.");
        }
    }
}
