#!/bin/sh
#
# receiver - receive video stream from ip camera and create multi
#

if [ $# -ne 1 ]; then
    echo "Usage: $0 [listen_port]"
    exit 1
fi

nc -kl -p $1 | ffmpeg -i - -f rtp rtp://localhost:9000 > cam.sdp
