TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/loca/opencv \
        /usr/local/opencv2/ \
        /home/reimondo/.pyenv/include/python2.7/

LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching \
        -lopencv_objdetect -lopencv_superres -lopencv_videostab \
        -lopencv_calib3d -lopencv_features2d -lopencv_highgui \
        -lopencv_videoio -lopencv_imgcodecs -lopencv_video \
        -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann \
        -lopencv_core -lopencv_hal \
        -L/home/reimondo/.pyenv/lib -lpython2.7

SOURCES += \
    dispatcher.cc \
    extractor.cc \
    pywrapper/dbconn.c

HEADERS += \
    sugar.h \
    sugar/sugar.h \
    extractor.h \
    pywrapper/dbconn.h
