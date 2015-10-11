#include <string>
#include <list>
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>

#include "gdatatype.h"
#include "memcache.h"
#include "sugar/sugar.h"

using std::string;
using std::list;
using std::to_string;

MemCache::MemCache()
      : redis_sync_(io_service_)
{
    server_ip_ = "127.0.0.1";
    port_ = 6379;

    ip_ = boost::asio::ip::address::from_string(server_ip_);

    string errmsg;
    if (!redis_sync_.connect(ip_, port_, errmsg)) {
        LogError(errmsg.c_str());
        exit(1);
    }
}

bool MemCache::save(const PersonShot person_shot)
{
    RedisValue res;

    // cache mat of person shot - dimension + elements
    //
    string person_shot_matrix_id = "psm:" + person_shot.get_id();
    FloatArray mat_array = person_shot.get_mat();

    res = redis_sync_.command("RPUSH",
                              person_shot_matrix_id,
                              to_string(mat_array.dimension));
    if (res.isError()) {
        LogError(res.toString().c_str());
        exit(1);
    }

    for (size_t i = 0; i < mat_array.matrix.size(); ++i) {
        res = redis_sync_.command("RPUSH",
                                  person_shot_matrix_id,
                                  to_string(mat_array.matrix[i]));
        if (res.isError()) {
            LogError(res.toString().c_str());
            exit(1);
        }
    }

    // cache person shot meta
    //
    // convert vector<int> rect_ to string
    //  e.g. "10 10 20 20"
    vector<int> rect = person_shot.get_rect();
    string rect_in_str;
    for (size_t i = 0; i < rect.size(); ++i)
        rect_in_str = rect_in_str + to_string(rect[i]) + " ";
    rect_in_str.pop_back();

    string person_shot_id = "ps:" + person_shot.get_id();
    list<string> args = {
        person_shot_id,
        "cam_id", person_shot.get_cam_id(),
        "frame_id", person_shot.get_frame_id(),
        "frame_pos", to_string(person_shot.get_frame_pos()),
        "video_id", person_shot.get_video_id(),
        "proper_vector_id", person_shot_matrix_id,
        "rect", rect_in_str
    };
    res = redis_sync_.command("HMSET", args);
    if (res.isError()) {
        LogError(res.toString().c_str());
        exit(1);
    }

    return true;
}

bool MemCache::save(const VideoShot video_shot)
{
    RedisValue res;

    // custom id
    string video_shot_id = "vs:" + video_shot.get_id();
    string video_shot_binary_id = "vsb:" + video_shot.get_id();

    list<string> vs_args = {
        video_shot_id,
        "cam_id", video_shot.get_cam_id(),
        "format", video_shot.get_format(),
        "codec", video_shot.get_codec(),
        "fps", to_string(video_shot.get_fps()),
        "frames", to_string(video_shot.get_frames()),
        "start_time", video_shot.get_start_time(),
        "end_time", video_shot.get_end_time(),
        "binary", video_shot_binary_id
    };

    list<string> vsb_args = {
        video_shot_binary_id,
        "fullpath", video_shot.get_fullpath(),
    };

    // save video shot in redis hash
    res = redis_sync_.command("HMSET", vs_args);
    if (res.isError()) {
        LogError(res.toString().c_str());
        exit(1);
    }

    // save binary of video
    res = redis_sync_.command("HMSET", vsb_args);
    if (res.isError()) {
        LogError(res.toString().c_str());
        exit(1);
    }

    return true;
}


bool MemCache::save(const KeyframeShot key_frame_shot)
{
    RedisValue res;

    string keyframe_shot_id = "kf:" + key_frame_shot.get_id();

    list<string> kf_args = {
        keyframe_shot_id,
        "fullpath", key_frame_shot.get_fullpath()
    };

    res = redis_sync_.command("HMSET", kf_args);
    if (res.isError()) {
        LogError(res.toString().c_str());
        exit(1);
    }
}
