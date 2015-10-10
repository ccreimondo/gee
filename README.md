gee
===

##About back-end API

1. RESTful API
2. HTTP Methods: GET, POST
3. Data exchange: JSON
4. Time format: yyyy-MM-dd HH:mm:ss
5. `<>` 表示变量
5. 返回单个对象
参数说明:
```json
{
    "entrance": "/api/gee/feeds/",
    "count": 10,
    "targets": [t1, t2...]
}
```

###请求监控视频数据
`"entrance": "/api/gee/records"`

####获取某天的监控视频数据

`GET /api/gee/records/<date>`
E.x. `/api/gee/records/20151010`

返回列表中的对象
```json
{
    "id": "c0a831201510100910540055700",
    "format": "H264",
    "fps": 30,
    "frames": 800,
    "time_range": {
        "start": "2015-10-10 22:10:00",
        "end": "2015-10-10 22:10:10"
    },
    "camera": {
        "id": "c0a831",
        "ip": "192.168.113.1",
        "address": ""
    }
}
```

####请求单个监控视频数据

`GET /api/gee/records/<id>`


###请求监控视频流数据
`"entrance": "/api/gee/records"`

####请求监控视频流数据列表

`GET /api/gee/records/`

返回列表中的对象
```json
{
    "id": "c0a831",
    "camera": {
        "id": "c0a831",
        "ip": "192.168.113.1",
        "address": ""
    },
    "format": "H264",
    "fps": 30,
    "frames": 800,
}
```

####请求单个监控视频流数据

`GET /api/gee/records/<id>`

