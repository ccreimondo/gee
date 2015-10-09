#include "gdatatype.h"

#include <iostream>
using std::cout;
using std::endl;

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
    // TODO (@Xinqian Gu): Convert Mat to array (stored in vector<float>).
	FloatArray f;
    f.dimension = proper_vector_.rows;
    for (int i = 0; i < proper_vector_.rows; ++i) {
		f.matrix.push_back(proper_vector_.at<float>(i, 0));
    }

	return f;
}
