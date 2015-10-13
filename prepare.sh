#!/bin/sh


# for app:actor
mkdir -p log

mkdir -p example/videos

mkdir -p /tmp/gee/video && \
mkdir -p /tmp/gee/keyframes && \
ln -s /tmp/gee/video actor/static/video && \
ln -s /tmp/gee/keyframes actor/static/keyframes

mkdir -p actor/static/tmp/person-shots && \
mkdir -p actor/static/tmp/query-frames

echo "done"
