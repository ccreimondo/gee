#!/bin/sh
#
# debug - capture video stream from device to rtp
#

if [ $# -ne 1 ]; then
    echo "Usage: $0 [listen_port]"
    exit 1
fi

ffmpeg -re -f v4l2 -i /dev/video0 -an -f rtp rtp://localhost:9000 > cam.sdp

