#pragma once
#ifndef GDATATYPE_H
#define GDATATYPE_H

#include <string>
#include <vector>
#include <sstream>

#include <opencv2/opencv.hpp>

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
    PersonShot(string id, string cam_id, string video_id,
               size_t frame_pos, vector<int> rect, cv::Mat &proper_vector);
    ~PersonShot(){}

    string get_id() { return id_; }
    string get_cam_id() { return cam_id_; }
    string get_video_id() { return video_id_; }
    size_t get_frame_pos() { return frame_pos_; }
    // TODO (@Zhiqiang He): check bug
    vector<int> get_rect() { return rect_; }
    cv::Mat get_proper_vector() { return proper_vector_; }

    // covert Mat to float array
    FloatArray mat_to_array();
private:
    string id_, cam_id_, video_id_;
    size_t frame_pos_;
    vector<int> rect_;
    cv::Mat proper_vector_;
};

//
//  Data type for Persister:
//      VideoStreamMeta, VideoTime
//
struct VideoStreamMeta {
    string format;      // input video stream format, we prefer H.264
    size_t fps;         // we prefer 30
    int solution[2];    // we prefer 1280x720
};


struct VideoTime {
    string time_start, time_end;    // we prefer 10min per piece
};

#endif // GDATATYPE_H
