gee
===

##About back-end API

1. RESTful API
2. HTTP methods: GET, POST
3. Data exchange format: JSON
4. Time format: yyyy-MM-dd HH:mm:ss
5. `<>` 表示变量
5. 返回单个对象
参数说明:
```json
{
    "entrance": "http://localhost:5000/api/gee/feeds/",
    "count": 10,
    "targets": [t1, t2...]
}
```

###请求监控视频数据

`GET /api/gee/videoshots/date:<date>`
Ex. `/api/gee/videohosts/date:20151010`

返回列表中的对象
```json
{
    "id": "c0a831201510100910540055700",
    "format": "mkv",
    "codec": "h264",
    "filename": "id.format",
    "fps": 30,
    "frames": 800,
    "time_range": {
        "start": "2015-10-10 22:10:00",
        "end": "2015-10-10 22:10:10"
    },
    "camera": {
        "ip": "192.168.3.1",
        "address": ""
    }
}
```

####单个监控的数据

`GET <entrance><video_id>`

###人物目标提取

`GET /api/gee/personshots/<video_id>/<frame_pos>/`

返回列表中的对象
```json
{
    "filename": "person_shot_tmp_id.jpeg",  // using default static path
    "id": "person_shot_tmp_id",     // reserved, for extra API
    "scale": "3:8",
    "video_id": "video_id",
    "frame_pos": "frame_pos"
}
```

####获取单个人体目标

`GET <entrance>/<video_id>/<frame_pos>/<person_shot_tmp_id>`

###人体目标识别

`POST /api/gee/personshots/<person_shot_tmp_id>`

返回列表中的对象
```json
{
    "id": "person_shot_id",         // reserved
    "rect": "10 10 20 20",
    "frame": "frame_id",            // to fetch picture of frame 
    "frame_pos": "frame_pos",
    "video_shot": {
        "fps": 30,
        "frames": 800,
        "codec": "h264",
        "format": "mkv",
        "time_range": {
            "start": "2015-10-10 22:10:00",
            "end": "2015-10-10 22:10:10"
        }
    },
    "camera": {
        "ip": "ip",
        "address": ""
    }
}
```

###请求关键帧

`GET <entrance><keyframe_id>`

###请求监控视频流数据

`GET /api/gee/feeds/`

返回列表中的对象
```json
{
    "id": "c0a80301",
    "camera": {
        "ip": "192.168.3.1",
        "address": ""
    },
    "format": "",
    "codec": "h264",
    "fps": 30,
    "frames": 800
}
```

####请求单个监控视频流数据

`GET <entrance><id>`

