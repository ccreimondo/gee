TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += /usr/loca/opencv \
        /usr/local/opencv2/ \
        /usr/local/boost_1_59_0

LIBS += -L/usr/local/lib -lpthread \
        -L/usr/local/lib -lopencv_shape -lopencv_stitching \
        -lopencv_objdetect -lopencv_superres -lopencv_videostab \
        -lopencv_calib3d -lopencv_features2d -lopencv_highgui \
        -lopencv_videoio -lopencv_imgcodecs -lopencv_video \
        -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann \
        -lopencv_core -lopencv_hal \
        -L/usr/local/boost_1_59_0/lib -lboost_system \

SOURCES += \
    dispatcher.cc \
    extractor.cc \
    redisclient/impl/redisasyncclient.cpp \
    redisclient/impl/redisclientimpl.cpp \
    redisclient/impl/redisparser.cpp \
    redisclient/impl/redissyncclient.cpp \
    redisclient/impl/redisvalue.cpp \
    gdatatype.cc \
    videostreamhandler.cc \
    sugar/sugar.cc \
    gdebug.cpp \
    getfeature.cpp \
    memcache.cpp

HEADERS += \
    sugar.h \
    sugar/sugar.h \
    extractor.h \
    redisclient/config.h \
    redisclient/redisasyncclient.h \
    redisclient/redisbuffer.h \
    redisclient/redisclient.h \
    redisclient/redisparser.h \
    redisclient/redissyncclient.h \
    redisclient/redisvalue.h \
    redisclient/version.h \
    redisclient/impl/redisclientimpl.h \
    gdatatype.h \
    videostreamhandler.h \
    gdebug.h \
    getfeature.h \
    memcache.h

OTHER_FILES += \
    PCA.xml
