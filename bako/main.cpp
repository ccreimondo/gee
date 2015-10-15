#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include "src/sugar/sugar.h"
#include "src/videostreamhandler.h"
#include "src/gdatatype.h"
#include "src/sugar/gdebug.h"

using namespace cv;
using std::string;
using std::vector;
using std::to_string;

void Dispatcher();
void Test();

int main(int argc, char *argv[])
{

    // Test();

    if (argc != 2) {
        // comment
        char buf[1024];
        sprintf(buf, "Simple entrance to experience and test.");
        sprintf(buf, "%s Data will be cached into redis.", buf);
        sprintf(buf, "%s Keyframes and videos will be saved into /tmp/gee.\n", buf);
        fprintf(stdout, "%s\n", buf);
        // usage
        fprintf(stdout, "\nUsage: %s {input_video_file}\n\n", argv[0]);
        exit(0);
    }

    IPCamera fake_ip_camera("192.168.113.147", "SEC 113");
    VideoStreamHandler(argv[1], fake_ip_camera);

    exit(0);
}

void Test()
{
    const string kDirPrefix = "/media/reimondo/HDD/Workspace/Projects/gee/";
    string video_stream_addr;

    // video stream from raspberry pi
    // video_stream_addr += kDirPrefix + "cam.sdp";

    // static video files for debugging
    vector<string> video_list;
    // video_list.push_back("gjw_x264.mp4");
    video_list.push_back("TEST.mkv");
    // video_list.push_back("DSC_4557_x264.mkv");
    // video_list.push_back("DSC_4549_x264.mkv");
    // video_list.push_back("DSC_4549_x264.mp4");
    // video_list.push_back("DSC_4565_x264.mp4");
    // video_list.push_back("DSC_4565_x264.mkv");
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
