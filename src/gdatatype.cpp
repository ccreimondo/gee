#include <string>

#include <opencv2/opencv.hpp>
#include "gdatatype.h"

using std::string;

PersonShot::PersonShot(string id, string cam_id, string video_id,
                       size_t frame_pos, vector<int> rect,
                       cv::Mat &proper_vector)
{
    id_ = id;
    cam_id_ = cam_id;
    video_id_ = video_id;
    frame_pos_ = frame_pos;
    rect_ = rect;
    proper_vector_ = proper_vector;
}

FloatArray PersonShot::mat_to_array()
{
	FloatArray f;
    f.dimension = proper_vector_.rows;
    for (int i = 0; i < proper_vector_.rows; ++i) {
		f.matrix.push_back(proper_vector_.at<float>(i, 0));
    }

	return f;
}

VideoShot::VideoShot(const string &video_id, const string &cam_id,
                     const size_t fps, const size_t frames, const string &codec,
                     const string &start_time, const string &end_time,
                     const string &path, const string &filename)
{
    cam_id_ = cam_id;

    // video meta
    video_id_ = video_id;
    fps_ = fps;
    frames_ = frames;
    codec_ = codec;

    // video extra meta
    start_time_ = start_time;
    end_time_ = end_time;
    filename_ = filename;
    path_ = path;
}
