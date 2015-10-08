#include <iostream>
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
using std::cout;
using std::endl;

MemCache::MemCache(boost::asio::io_service &io_service)
      : redis_sync_(io_service)
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

bool MemCache::save(PersonShot person_shot)
{
    RedisValue res;

    // save mat in redis list - dimension + elements
    string list_id = "mat:" + person_shot.get_id();
    FloatArray mat_array = person_shot.mat_to_array();
    res = redis_sync_.command("RPUSH",
                              list_id, to_string(mat_array.dimension));
    if (res.isError()) {
        LogError(res.toString().c_str());
        exit(1);
    }
    for (int i = 0; i < mat_array.matrix.size(); ++i)
        res = redis_sync_.command("RPUSH",
                                  list_id, to_string(mat_array.matrix[i]));

    // convert vector<int> rect_ to string
    vector<int> rect = person_shot.get_rect();
    string rect_in_str;
    for (int i = 0; i < rect.size(); ++i)
        rect_in_str = rect_in_str + to_string(rect[i]) + " ";
    rect_in_str.pop_back();

    list<string> args = {
        person_shot.get_id(),
        "cam_id", person_shot.get_cam_id(),
        "frame_pos", to_string(person_shot.get_frame_pos()),
        "proper_vector_id", list_id,
        "rect", rect_in_str
    };
    // save PersonShot in redis hash
    res = redis_sync_.command("HMSET", args);
    if (res.isError()) {
        LogError(res.toString().c_str());
        exit(1);
    }
}

// bool MemCache::save(VideoShot video_shot)
// {
//
// }
