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

using std::string;

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
