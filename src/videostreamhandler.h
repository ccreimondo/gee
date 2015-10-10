#ifndef VIDEOSTREAMHANDLER_H
#define VIDEOSTREAMHANDLER_H

//
// VideoStreamHandler: focus on handling one video stream from
//      the IP camera.
//
// This file has the following parts:
//  P1: VideoCutter for Persister and Extractor.
//  P2: Forward video stream to Forwarder.
//

#include <string>

#include <opencv2/opencv.hpp>
#include "gdatatype.h"
#include "memcache.h"

using std::string;

// Do memory cache:
//  Video is saved to disk and its meta is
//  saved to redis.
//
class VideoCacher {
public:
    VideoCacher();
    ~VideoCacher() {}

    void handler(const string &cam_id,
                 const string &video_id,
                 const VideoTime video_time,
                 const VideoStreamMeta video_stream_meta,
                 const cv::Mat &frame);

    // save video and reset the instance
    void release();

private:
    string cam_id_, video_id_;
    VideoTime video_time_;
    VideoStreamMeta video_stream_meta_;
    bool is_init_;  // default false

    string path_;    // default /tmp/gee/video/
    string filename_;// filename of video record
    size_t frames_counter_; // to counter frames

    // create a VideoWriter instance
    cv::VideoWriter writer_;

    // to do video shot memcache
    MemCache memcache_;

    // init the instance
    void init(const string &cam_id,
              const string &video_id,
              VideoTime video_time,
              VideoStreamMeta video_stream_meta);
    bool is_init() { return is_init_; }
};

// entity
//
void VideoStreamHandler(const string &sdp_addr, const string &cam_id);

// forward video stream to front-end
//
void VideoForwarder(const string &cam_id,
                    const string &video_id,
                    const VideoTime video_time,
                    const VideoStreamMeta video_stream_meta,
                    const cv::Mat &frame);

#endif // VIDEOSTREAMHANDLER_H
