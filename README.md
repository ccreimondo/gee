gee
===

##About back-end API

1. RESTful API
2. HTTP Methods: GET
3. Data exchange: JSON
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

####请求监控视频数据列表
`GET /api/gee/videoshots/date:<date>`
Ex. `/api/gee/videohosts/date:20151010`

返回列表中的对象
```json
{
    "id": "c0a831201510100910540055700",
    "format": "ogg",
    "codec": "h264",
    "filename": "id.format",
    "fps": 30,
    "frames": 800,
    "time_range": {
        "start": "2015-10-10 22:10:00",
        "end": "2015-10-10 22:10:10"
    },
    "camera": {
        "id": "c0a80301",
        "ip": "192.168.3.1",
        "address": ""
    }
}
```

####请求单个监控视频数据

`GET <entrance> + <filename>`

###人物目标提取

`GET /api/gee/personshots/vid:<vid>/<frame_pos>`

返回列表中的对象
```json
{
}
```


###人体目标识别

`GET /api/gee/personshots/<person_shot_id>`

返回列表中的对象
```json
{
}
```

###请求监控视频流数据

####请求监控视频流数据列表

`GET /api/gee/feeds`

返回列表中的对象
```json
{
    "id": "c0a80301",
    "camera": {
        "id": "c0a80301",
        "ip": "192.168.3.1",
        "address": ""
    },
    "format": "H264",
    "fps": 30,
    "frames": 800,
}
```

####请求单个监控视频流数据

`GET <entrance> + <id>`

