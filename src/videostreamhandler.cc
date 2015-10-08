#include <string>

#include <opencv2/opencv.hpp>
#include "videostreamhandler.h"
#include "extractor.h"
#include "sugar/sugar.h"
#include "gdebug.h"

using namespace cv;
using std::string;

const int kBufferSize = 100;

string GetVideoID();
string GetSysTimeNow();

void VideoStreamHandler(const string &sdp_addr, const string &cam_id)
{
    // create video stream capture
    VideoCapture cap(sdp_addr);
    if (!cap.isOpened()) {
        LogError("Fail to open video stream.");
        exit(1);
    }

    // fill video stream meta
    VideoStreamMeta video_stream_meta;
    video_stream_meta.format = "H264";
    video_stream_meta.fps = (size_t)cap.get(CV_CAP_PROP_FPS);
    video_stream_meta.solution[0] = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
    video_stream_meta.solution[1] = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);

#ifndef NOGDEBUG
    cout << "-------------VIDEO STREAM META-----------" << endl;
    cout << "SOURCE: " << sdp_addr << endl
         << "FORMAT: " << video_stream_meta.format << endl
         << "FPS: " << video_stream_meta.fps << endl
         << "SOLUTION: " << video_stream_meta.solution[0] << " "
         << video_stream_meta.solution[1] << endl;
    cout << "-----------------------------------------" << endl;
#endif

    // init timestamp
    double timestamp_before = cap.get(CV_CAP_PROP_POS_MSEC);
    double timestamp_after = cap.get(CV_CAP_PROP_POS_MSEC);

    // init counter and prepare some vars
    size_t frame_counter = 0;
    string video_id;
    VideoTime video_time;
    Mat curr_frame;

    // init handler
    Extractor extractor;
    VideoCacher videocacher;

    while (1) {
        if (!cap.read(curr_frame)) {
            LogError("Unable to read next frame.");
            LogError("Exiting");
            exit(1);
        }

        // set frame counter
        frame_counter++;

        // init some vars for the first frame
        if (cap.get(CV_CAP_PROP_POS_FRAMES) <= 1) {
            video_id = GetVideoID();
            video_time.time_end = video_time.time_start =
                    GetSysTimeNow();
        }

        // cut per 10mins, 600000ms
        timestamp_after = cap.get(CV_CAP_PROP_POS_MSEC);
        if (timestamp_after - timestamp_before >= 600000) {
            // get end time for the current video
            video_time.time_end = GetSysTimeNow();
            // get id and start time for the new video
            video_id = GetVideoID();
            video_time.time_start = GetSysTimeNow();

            // update timestamp before
            timestamp_before = cap.get(CV_CAP_PROP_POS_MSEC);
            // reset frame counter
            frame_counter = 0;
        }

        // asynchronous thread
        // cache video stream
        videocacher.handler(cam_id, video_id,
                            video_time, video_stream_meta,
                            curr_frame);
        extractor.handler(curr_frame, cam_id, video_id, frame_counter);

        VideoForwarder(cam_id, video_id,
                       video_time, video_stream_meta,
                       curr_frame);

        waitKey(1);
    }
}

string GetVideoID()
{
    string fmt = "%d%m%Y%I%M";  // e.g. 201510072210

    return GetTimeNow(fmt);
}

string GetSysTimeNow()
{
    string fmt = "%d%m%Y%I%M%S";    // e.g. 20151007221022

    return GetTimeNow(fmt);
}

void VideoForwarder(const string &cam_id,
                    const string &video_id,
                    const VideoTime video_time,
                    const VideoStreamMeta video_stream_meta,
                    const Mat &frame)
{
    //cout << "This is video forwarder" << endl;
}

VideoCacher::VideoCacher()
{
    is_init_ = false;
    path = "/tmp/gee/video/";
}

void VideoCacher::init(const string &cam_id,
                       const string &video_id,
                       VideoTime video_time,
                       VideoStreamMeta video_stream_meta)
{
    // create video meta
    cam_id_ = cam_id;
    video_id_ = video_id;
    video_time_.time_start = video_time.time_start;
    video_stream_meta_ = video_stream_meta;

    // open a file to write video stream
    char filename[kBufferSize];
    sprintf(filename, "%s%s%s.%s",
            path.c_str(), IP2HexStr(cam_id).c_str(), video_id.c_str(), "H264");
    cout << filename << endl;
    Size v_size(video_stream_meta_.solution[0],
                video_stream_meta_.solution[1]);
    writer.open(filename, CV_FOURCC('X', '2', '6', '4'),
                video_stream_meta_.fps, v_size);
    if (!writer.isOpened()) {
        LogError("Fail to open /srv/gee/video.");
        exit(1);
    }

    // update init stat
    is_init_ = true;
}

void VideoCacher::handler(const string &cam_id,
                          const string &video_id,
                          const VideoTime video_time,
                          const VideoStreamMeta video_stream_meta,
                          const Mat &frame)
{
    // auto init when created
    if (!is_init()) {
        init(cam_id, video_id, video_time, video_stream_meta);
    }

    // new video piece come
    if (cam_id != cam_id_) {
        video_time_.time_end = video_time.time_end;
        // TODO (@Zhiqiang He): save meta to redis

        // finish handling last video piece and release resource
        release();
        init(cam_id, video_id, video_time, video_stream_meta);
    } else {
        // continue to write video to disk
        cout << "Write a frame" << endl;
        writer << frame;
    }
}

void VideoCacher::release()
{
    is_init_ = false;
    writer.release();
}
