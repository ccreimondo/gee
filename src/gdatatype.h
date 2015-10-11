#ifndef GDATATYPE_H
#define GDATATYPE_H

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include "sugar/sugar.h"

using std::string;
using std::vector;

//
//  Data type for Extractor:
//      FloatArray, PersonShot
//
struct FloatArray {
    int dimension;  // default 100
    vector<float> matrix;
};

class PersonShot {
public:
    PersonShot();
    // id (char[27]): cam_id + video_id + frame_pos + sequence
    // cam_id (char[8]): COA87101 (192.168.113.1)
    // video_id (char[12]): 201510072210 (2015-10-07 22:10)
    // rect: point_1 (rect[0], rect[1]), point_2 (rect[2], rect[3])
    PersonShot(const size_t sequence,
               const string cam_id,
               const string video_id,
               const string frame_id,
               const size_t frame_pos,
               const vector<int> &rect,
               const cv::Mat &proper_vector);
    ~PersonShot(){}

    string get_id() const  { return id_; }
    string get_cam_id() const { return cam_id_; }
    string get_video_id() const { return video_id_; }
    string get_frame_id() const { return frame_id_; }
    size_t get_frame_pos() const { return frame_pos_; }
    vector<int> get_rect() const { return rect_; }
    // covert Mat to float array
    FloatArray get_mat() const ;
private:
    // hex(ip) + %Y%m%d%I%M%S + frame_pos(len=5) + sequence(len=2)
    string id_,
           cam_id_,
           video_id_,
           frame_id_;
    size_t frame_pos_;
    vector<int> rect_;
    cv::Mat proper_vector_;
};

//
//  Data type for VideoCacher:
//      VideoStreamMeta, VideoTime, VideoShot
//
struct VideoStreamMeta {
    string codec;      // input video stream format, we prefer H.264
    size_t fps;         // we prefer 30
    int solution[2];    // we prefer 1280x720
};

struct VideoTime {
    string time_start, time_end;    // 10min per piece
                                    // time format: %Y%m%d%I%M%S
};

// video snapshot object
//
class VideoShot {
public:
    VideoShot(const string &video_id, const string &cam_id,
              const size_t fps, const size_t frames,
              const string &format, const string &codec,
              const string &start_time, const string &end_time,
              const string &path, const string &filename);
    ~VideoShot() {}

    string get_id() const { return id_; }
    string get_cam_id() const { return cam_id_; }
    string get_format() const { return format_; }
    string get_codec() const { return codec_; }
    size_t get_fps() const { return fps_; }
    size_t get_frames() const { return frames_; }
    string get_start_time() const { return start_time_; }
    string get_end_time() const { return end_time_; }
    string get_fullpath() const { return path_ + filename_; }

private:
    string id_;     // hex(ip) + %Y%m%d%I%M%S
    string format_, codec_; // codec for en/decoding, format for container
    size_t fps_, frames_;
    string start_time_, end_time_;
    string path_, filename_;
    string cam_id_;
};

// key frame object
//
class KeyframeShot {
public:
    KeyframeShot(const string &video_id,
                 const string &cam_id,
                 const size_t frame_pos,
                 const string &path,
                 const string &filename,
                 const cv::Mat &frame);
    ~KeyframeShot() {}

    string get_id() const { return id_; }
    string get_fullpath() const { return path_ + filename_; }
    vector<float> get_frame_mat();

private:
    string id_;     // hex(ip) + %Y%m%d%I%M%S + frame_pos (len=5)
    string path_, filename_;
    cv::Mat frame_;
};

// camera object
//
class IPCamera {
public:
    IPCamera(const string &ip,
             const string &address);
    ~IPCamera() {}

    string get_id() const { return id_; }
    string get_ip() const { return ip_; }
    string get_address() const { return address_; }

private:
    string id_; // hex(ip)
    string ip_;
    string address_;    // physical address
};

#endif // GDATATYPE_H
