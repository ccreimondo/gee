#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include "gdatatype.h"
#include "sugar/sugar.h"

using std::string;
using std::vector;
using std::to_string;


PersonShot::PersonShot(const size_t sequence,
                       const string cam_id,
                       const string video_id,
                       const string frame_id,
                       const size_t frame_pos,
                       const vector<int> &rect,
                       const cv::Mat &proper_vector)
{
    // TODO (@Zhiqiang He): person_shot_counter must in format %d{5}
    id_ = cam_id + video_id +
          FormatUnsignedInt(frame_pos, 5) +
          FormatUnsignedInt(sequence, 2);
    cam_id_ = cam_id;
    video_id_ = video_id;
    frame_id_ = frame_id;
    frame_pos_ = frame_pos;
    rect_ = rect;
    proper_vector_ = proper_vector;
}

FloatArray PersonShot::get_mat() const
{
	FloatArray f;
    f.dimension = proper_vector_.rows;
    for (int i = 0; i < proper_vector_.rows; ++i) {
		f.matrix.push_back(proper_vector_.at<float>(i, 0));
    }

	return f;
}

VideoShot::VideoShot(const string &video_id, const string &cam_id,
                     const size_t fps, const size_t frames,
                     const string &format, const string &codec,
                     const string &start_time, const string &end_time,
                     const string &path, const string &filename)
{
    id_ = cam_id + video_id;

    fps_ = fps;
    frames_ = frames;
    format_ = format;
    codec_ = codec;
    start_time_ = start_time;
    end_time_ = end_time;
    cam_id_ = cam_id_;

    // temp solution
    filename_ = filename;
    path_ = path;
}

KeyframeShot::KeyframeShot(const string &video_id,
                           const string &cam_id,
                           const size_t frame_pos,
                           const string &path,
                           const string &filename,
                           const cv::Mat &frame)
{
    // frame_pos must have in format %d{5}
    id_ = cam_id + video_id + FormatUnsignedInt(frame_pos, 5);
    frame_ = frame.clone();
    path_ = path;
    filename_ = filename;
}

vector<float> KeyframeShot::get_frame_mat()
{
    // TODO (@Zhiqiang He): convert Mat to array
    return vector<float>();
}

IPCamera::IPCamera(const string &ip, const string &address)
{
    id_ = IP2HexStr(ip);
    ip_ = ip;
    address_ = address;
}
