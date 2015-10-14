CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += \
        /usr/local/include/opencv2 \
        /usr/include/boost \
        /usr/include/python2.7 \


LIBS += `pkg-config --libs opencv python2` \
        -L/usr/lib -lpthread \
        -L/usr/lib -lboost_system \

SOURCES += \
    src/redisclient/impl/redisasyncclient.cpp \
    src/redisclient/impl/redisclientimpl.cpp \
    src/redisclient/impl/redisparser.cpp \
    src/redisclient/impl/redissyncclient.cpp \
    src/redisclient/impl/redisvalue.cpp \
    src/memcache.cpp \
    src/sugar/gdebug.cpp \
    src/sugar/sugar.cpp \
    src/extractor.cpp \
    src/gdatatype.cpp \
    src/videocacher.cpp \
    src/videostreamhandler.cpp \
    src/galgorithm.cpp \
    src/RBML/getfeature.cpp \
    main.cpp

HEADERS += \
    src/redisclient/config.h \
    src/redisclient/redisasyncclient.h \
    src/redisclient/redisbuffer.h \
    src/redisclient/redisclient.h \
    src/redisclient/redisparser.h \
    src/redisclient/redissyncclient.h \
    src/redisclient/redisvalue.h \
    src/redisclient/version.h \
    src/redisclient/impl/redisclientimpl.h \
    src/sugar/gdebug.h \
    src/sugar/sugar.h \
    src/extractor.h \
    src/gdatatype.h \
    src/videostreamhandler.h \
    src/memcache.h \
    src/videocacher.h \
    src/galgorithm.h \
    src/RBML/getfeature.h

OTHER_FILES += \
    src/RBML/PCA.xml \
