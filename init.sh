#!/bin/sh
#
# init - dump video from device
#

if [ $# -ne 2 ]; then
    echo "Usage: $0 [destination_ip] [port]"
    exit 1
fi

ffmpeg -f v4l2 -i /dev/video0 -f h264 -b:v 5m -r 30 - 2>/dev/null | nc $1 $2

