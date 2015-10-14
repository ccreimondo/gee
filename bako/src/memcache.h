#ifndef MEMCACHE_H
#define MEMCACHE_H

#include <string>
#include <boost/asio.hpp>

#include "redisclient/redissyncclient.h"
#include "gdatatype.h"

using std::string;

//
// `redis-client` wrapper for the project.
//
// Support data struct:
//  PersonShot, VideoShot
//
// Design pattern:
//  1. Hide details.
//  2. Elegant error handler.
//  3. Safe? -_>-
//
// @Zhiqiang He
//
class MemCache
{
public:
    MemCache();
    ~MemCache() {}

    // save
    //
    bool save(const PersonShot person_shot);
    bool save(const VideoShot video_shot);
    bool save(const KeyframeShot key_frame_shot);

    // callback
    //
    // void on_ps_save(const RedisValue &value);
    // void on_vs_save(const RedisValue &value);

private:
    // db connection about
    string server_ip_;  // default 127.0.0.1
    unsigned short port_;    // default 6379
    // ip
    boost::asio::ip::address ip_;

    // boost io_service
    boost::asio::io_service io_service_;

    // names of channes which are used to push commands
    string vs_cmd_ch_name_, ps_cmd_ch_name_;  // default cmd.vs, cmd.ps
    // channel clients
    // RedisAsyncClient cmd_ch_vs_, cmd_ch_ps_;

    // synchronous redis client
    RedisSyncClient redis_sync_;
};

#endif // MEMCACHE_H
