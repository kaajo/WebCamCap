#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T20:27:31
#
#-------------------------------------------------

QT       += gui

TARGET = WebCamCapPrimitives
TEMPLATE = lib

DEFINES += WEBCAMCAPPRIMITIVES_LIBRARY

SOURCES += \
    line.cpp \
    marker.cpp \
    camerasettings.cpp \
    roomsettings.cpp \
    frame.cpp \
    animation.cpp

HEADERS +=\
        webcamcapprimitives_global.h \
    line.h \
    marker.h \
    camerasettings.h \
    roomsettings.h \
    frame.h \
    animation.h

CONFIG += link_pkgconfig

PKGCONFIG += opencv

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

unix {
    target.path = /usr/lib
    INSTALLS += target
}
