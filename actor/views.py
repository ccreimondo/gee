# coding=utf-8

import os
import cv2
import redis
import logging
import numpy as np

from actor import app
from datetime import datetime
from flask import jsonify, g, abort, send_file, request

# default configuration
APP_NAME = "Actor"
LOG_DIR = "/tmp/actor/log/"
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
    for idx, vid in enumerate(video_shots):
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
            "id": video_shot["cam_id"],
            "ip": video_shot["cam_id"],
            "address": "",
            "index": idx
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


def fetch_frame(vid, frame_pos):
    """Fetch frame from video shot with given vid and frame position.

    Returns:
        Mat frame
    """
    vs_id = "vs:{}".format(vid)
    vsb_id = "vsb:{}".format(vid)
    video_shot = get_redis().hgetall(vs_id)
    video_shot_where = get_redis().hgetall(vsb_id)
    if not video_shot_where or not video_shot:
        return None
    video_shot_full_path = os.path.join(video_shot_where["path"],
                                        video_shot_where["filename"])

    vs_frames = video_shot["frames"]
    if int(frame_pos) > int(vs_frames):
        return None

    cap = cv2.VideoCapture(video_shot_full_path)
    cap.set(cv2.CAP_PROP_POS_FRAMES, float(frame_pos))

    while cap.isOpened():
        ret, frame = cap.read()
        if ret:
            return frame

    return None


def cache_query_frame(frame):
    """Cache frame to redis.

    Returns:
        key
    """
    pass


def inside(r, q):
    rx, ry, rw, rh = r
    qx, qy, qw, qh = q
    return rx > qx and ry > qy and rx + rw < qx + qw and ry + rh < qy + qh


def human_detect(frame):
    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

    found_rects, w = hog.detectMultiScale(frame, 0, (8, 8), (32, 32), 1.05, 2)
    found_rects_filtered = []
    for ri, r in enumerate(found_rects):
            for qi, q in enumerate(found_rects):
                if ri != qi and inside(r, q):
                    break
            else:
                found_rects_filtered.append(r)

    return found_rects_filtered


def extract_person_shots(frame):
    """Extract person shots from the given frame.

    Returns:
        list of person_shots
    """
    rectangles = human_detect(frame)
    person_shots = []
    for rect in rectangles:
        frame_n = np.copy(frame)
        (x, y, w, h) = rect
        person_shot = frame_n[y:y+h, x:x+w]
        person_shots.append(person_shot)
    return person_shots


# match
def match():
    pass


@app.route("/")
def hello():
    return "IBM Power Contest 2015 @Continue"


@app.route("/api/gee/feeds/")
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


@app.route("/api/gee/videoshots/<video_id>")
def get_gee_video_shot(video_id):
    abort(500)


@app.route("/api/gee/personshots/<vid>/<frame_pos>/")
def get_gee_person_shots(vid, frame_pos):
    res = {
        "entrance": "http://10.250.94.25:5000/static/tmp/person-shots/",
        "count": 0,
        "targets": []
    }
    target_frame = fetch_frame(vid, frame_pos)
    if target_frame is None:
        abort(404)  # frame not found
    # save the query frame to debug
    qf_id = "{}{:05d}".format(vid, int(frame_pos))
    qf_full_path = "{}{}.{}".format("actor/static/tmp/query-frames/",
                                    qf_id, "jpeg")
    cv2.imwrite(qf_full_path, target_frame)
    # return person shots rect
    person_shots = extract_person_shots(target_frame)
    for idx, ps in enumerate(person_shots):
        pst_id = "{}{:05d}{:02d}".format(vid, int(frame_pos), idx)
        # cache mat for searching next
        filename = "{}.{}".format(pst_id, "jpeg")
        full_path = "{}{}".format("actor/static/tmp/person-shots/", filename)
        cv2.imwrite(full_path, ps)
        target = {
            "filename": filename,
            "id": pst_id,
            "scale": "3:8",
            "parent": "{}.{}".format(qf_id, "jpeg")
        }
        res["targets"].append(target)
    res["count"] = len(res["targets"])
    return jsonify(res)


@app.route("/api/gee/personshots/<vid>/<frame_pos>/<person_shot_tmp_id>")
def get_gee_person_shot(vid, frame_pos, pst_id):
    abort(500)


@app.route("/api/gee/personshots/<person_shot_id>", methods=["GET", "POST"])
def get_gee_person_shots_archive(person_shot_id):
    if request.method == "GET":
        abort(500)

    # search person in database with target person shot
    if request.method == "POST":
        res = {
            "entrance": "http://10.250.94.25:5000/static/keyframes/",
            "count": 0,
            "targets": []
        }

    abort(500)


@app.route("/api/gee/keyframes/<keyframe_id>")
def get_gee_keyframes(keyframe_id):
    filename = "{}{}.{}".format("actor/static/keyframes/", keyframe_id, "jpeg")
    return send_file(filename, mimetype="image/jpeg")


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
