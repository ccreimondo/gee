TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += /usr/include/opencv \
        /usr/include/opencv2/ \
        /usr/local/boost_1_59_0 \
        /usr/include/python2.7

LIBS += -L/usr/lib -lpthread \
        -L/usr/lib -lopencv_stitching \
        -lopencv_objdetect -lopencv_superres -lopencv_videostab \
        -lopencv_calib3d -lopencv_features2d -lopencv_highgui \
        -lopencv_video \
        -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann \
        -lopencv_core \
        -L/usr/local/boost_1_59_0/lib -lboost_system \
        -L/usr/lib/python2.7

SOURCES += \
    redisclient/impl/redisasyncclient.cpp \
    redisclient/impl/redisclientimpl.cpp \
    redisclient/impl/redisparser.cpp \
    redisclient/impl/redissyncclient.cpp \
    redisclient/impl/redisvalue.cpp \
    gdebug.cpp \
    memcache.cpp \
    sugar/sugar.cpp \
    dispatcher.cpp \
    extractor.cpp \
    gdatatype.cpp \
    videocacher.cpp \
    videostreamhandler.cpp \
    galgorithm.cpp \
    RBML/getfeature.cpp \

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
    memcache.h \
    videocacher.h \
    galgorithm.h \
    RBML/getfeature.h

OTHER_FILES += \
    RBML/PCA.xml \
    pywrapper/Makefile
