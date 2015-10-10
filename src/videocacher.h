#ifndef VIDEOCACHER_H
#define VIDEOCACHER_H

#include <string>

#include "gdatatype.h"
#include "memcache.h"
#include "gdatatype.h"

using std::string;

// Do memory cache:
//  Video is saved to disk and its meta is
//  saved to redis.
//
class VideoCacher {
public:
    VideoCacher();
    ~VideoCacher() {}

    // entity
    void handler(const string &cam_id,
                 const string &video_id,
                 const VideoTime video_time,
                 const VideoStreamMeta video_stream_meta,
                 const size_t frames_counter,
                 const cv::Mat &frame);

    // save video and reset the instance
    void release();

private:
    string cam_id_, video_id_;
    VideoTime video_time_;
    string video_start_time_;
    string video_end_time_;
    VideoStreamMeta video_stream_meta_;

    // where to save video
    string path_;    // default /tmp/gee/video/


    // to do video shot memcache
    MemCache memcache_;

    // tmp vars, they must be reset every release time
    bool is_init_;              // default false
    string filename_;           // filename of video record
    size_t frames_counter_;     // to counter frames
    cv::VideoWriter writer_;    // VideoWriter instance


    // init a new video cache
    void init(const string &cam_id,
              const string &video_id,
              VideoTime video_time,
              VideoStreamMeta video_stream_meta);
    bool is_init() { return is_init_; }

    // to update part data
    void update(size_t frames_counter,
                VideoTime video_time);
};

#endif // VIDEOCACHER_H
