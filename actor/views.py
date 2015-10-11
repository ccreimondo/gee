# coding=utf-8

import os
import redis
import logging

from actor import app
from datetime import datetime
from flask import jsonify, g

# default configuration
APP_NAME = "Actor"
LOG_DIR = "log"
REDIS_HOST = "127.0.0.1"
REDIS_PORT = 6379
REDIS_DB = 0

# load configuration
app.config.from_object(__name__)


def connect_redis():
    """Connect to the specific database."""
    conn = redis.StrictRedis(host=app.config["REDIS_HOST"],
                             port=app.config["REDIS_PORT"],
                             db=app.config["REDIS_DB"])
    return conn


def get_redis():
    if not hasattr(g, "redis"):
        g.redis = connect_redis()
    return g.redis


def fetch_feeds_list():
    """Fetch feeds from redis.

    Returns:
        list of feeds
    """
    pass


def ipv4_to_hex(ipv4):
    hexes = [format(int(x), "02x") for x in ipv4.split('.')]
    return ''.join(hexes)


def hex_to_ipv4(hex):
    pass


def fetch_records_list(date):
    """Fetch records from redis by date.
    Args:
        date: yyyyMMdd

    Returns:
        list of records
    """
    r = get_redis()
    video_shots = r.keys("vs:*")
    video_shorts_f = []
    for vid in video_shots:
        video_shot = r.hgetall(vid)
        id_f = vid.split(':')[1]
        raw_time_fmt = "%Y%m%d%H%M%S"
        time_fmt = "%Y-%m-%d %H:%M:%S"
        start_time = datetime.strptime(video_shot["start_time"], raw_time_fmt)
        end_time = datetime.strptime(video_shot["end_time"], raw_time_fmt)
        time_range_f = {
            "start_time": start_time.strftime(time_fmt),
            "end_time": end_time.strftime(time_fmt)
        }
        camera_f = {
            "id": ipv4_to_hex(video_shot["cam_id"]),
            "ip": video_shot["cam_id"],
            "address": ""
        }
        video_shot_f = {
            "id": id_f,
            "format": video_shot["format"],
            "fps": str(video_shot["fps"]),
            "frames": str(video_shot["frames"]),
            "codec": video_shot["codec"],
            "time_range": time_range_f,
            "camera": camera_f,
            "filename": ''.join([id_f, ".", video_shot["format"]])
        }
        video_shorts_f.append(video_shot_f)
    return video_shorts_f


@app.route("/")
def hello():
    return "IBM Power Contest 2015 @Continue"


@app.route("/api/gee/feeds")
def get_gee_feeds():
    res = {
        "entrance": "http://10.250.94.25:5000/api/gee/feeds/",
        "count": 0,
        "targets": []
    }
    return jsonify(res)


@app.route("/api/gee/videoshots/date:<date>")
def get_gee_video_shots(date):
    targets = fetch_records_list(date)
    res = {
        "entrance": "http://10.250.94.25:5000/static/video/",
        "count": 0,
        "targets": targets
    }
    res["count"] = len(res["targets"])
    return jsonify(res)


@app.route("/api/gee/videoshots/<vid>")
def get_gee_video_shot(vid):
    return "error"


@app.route("/api/gee/personshots/<person_shot_id>")
def get_gee_person_shots_archive(person_shot_id):
    pass


@app.route("/api/gee/personshots/vid:<vid>/<frame_pos>")
def get_gee_person_shots(vid, frame_pos):
    pass


def error_log_file_handler():
    # create error log file
    log_dir = app.config["LOG_DIR"]
    log_name = '.'.join([APP_NAME.lower(), "error", "log"])
    if "ERROR_LOG" in app.config:
        log_name = app.config["ERROR_LOG"]
    error_log = os.path.join(log_dir, log_name)

    fh = logging.FileHandler(error_log)
    fh.setLevel(logging.ERROR)
    formatter = logging.Formatter(fmt="%(asctime)s %(levelname)s: %(message)s"
                                      "[in %(pathname)s:%(lineno)d]",
                                  datefmt="%Y-%m-%d %H:%M:%S")
    fh.setFormatter(formatter)
    return fh


app.logger.addHandler(error_log_file_handler())
