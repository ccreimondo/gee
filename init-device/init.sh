#!/bin/sh
#
# init - dump video from device
#

if [ $# -ne 2 ]; then
    echo "Usage: $0 [destination_ip] [port]"
    exit 1
fi

ffmpeg -re -f v4l2 -framerate 60 -i /dev/video0 -an -f h264 - | nc $1 $2

