#include <string>

#include <opencv2/opencv.hpp>
#include "videocacher.h"
#include "gdatatype.h"
#include "memcache.h"
#include "gdebug.h"

using namespace cv;
using std::string;

VideoCacher::VideoCacher()
{
    is_init_ = false;
    path_ = "/tmp/gee/video/";
    filename_ = "";
    frames_counter_ = 0;
}

void VideoCacher::init(const string &cam_id,
                       const string &video_id,
                       VideoTime video_time,
                       VideoStreamMeta video_stream_meta)
{
    // init new video meta
    cam_id_ = cam_id;
    video_id_ = video_id;
    video_stream_meta_ = video_stream_meta;
    video_start_time_ = video_time.time_start;

    // open a file to write video stream
    filename_ = path_ + IP2HexStr(cam_id) + video_id + ".H264";
    Size v_size(video_stream_meta_.solution[0],
                video_stream_meta_.solution[1]);
    // BUG (@Zhiqiang He): nothing saved
    writer_.open(filename_, CV_FOURCC('X', '2', '6', '4'),
                video_stream_meta_.fps, v_size);
    if (!writer_.isOpened()) {
        LogError("Fail to open /tmp/gee/video.");
        exit(1);
    }

    // new video cache is created sucessfully
    is_init_ = true;
}

void VideoCacher::update(size_t frames_counter,
                         VideoTime video_time)
{
    frames_counter_ = frames_counter;
    video_end_time_ = video_time.time_end;
}

void VideoCacher::handler(const string &cam_id,
                          const string &video_id,
                          const VideoTime video_time,
                          const VideoStreamMeta video_stream_meta,
                          const size_t frames_counter,
                          const Mat &frame)
{
    // create the first video piece
    if (!is_init()) {
        init(cam_id, video_id, video_time, video_stream_meta);
    }

    // update some data of video piece
    update(frames_counter, video_time);

    // new video piece come
    if (cam_id != cam_id_) {
        // finish handling last video piece and release resource
        release();

        // create a new video piece
        init(cam_id, video_id, video_time, video_stream_meta);
        update(frames_counter, video_time);
        writer_ << frame;
    } else {
        // continue
        writer_ << frame;
    }
}

void VideoCacher::release()
{
    if (is_init_) {
        // save video meta into redis
        VideoShot video_shot(video_id_, cam_id_,
                             video_stream_meta_.fps,
                             frames_counter_, "H264",
                             video_start_time_,
                             video_end_time_,
                             path_, filename_);
        memcache_.save(video_shot);

        // reset resouces
        filename_.clear();
        writer_.release();
        frames_counter_ = 0;
        is_init_ = false;
    }
}
