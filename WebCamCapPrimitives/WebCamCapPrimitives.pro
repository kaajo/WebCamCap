#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T20:27:31
#
#-------------------------------------------------

TARGET = WebCamCapPrimitives
TEMPLATE = lib

DEFINES += WEBCAMCAPPRIMITIVES_LIBRARY

SOURCES += \
    line.cpp \
    marker.cpp \
    camerasettings.cpp \
    roomsettings.cpp \
    frame.cpp \
    animation.cpp \
    wcctofbxexporter.cpp

HEADERS +=\
        webcamcapprimitives_global.h \
    line.h \
    marker.h \
    camerasettings.h \
    roomsettings.h \
    frame.h \
    animation.h \
    wcctofbxexporter.h


CONFIG += link_pkgconfig

PKGCONFIG += opencv

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#FBX_SDK

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/release/ -lfbxsdk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/debug/ -lfbxsdk
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/ -lfbxsdk

INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/include
