#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T16:36:06
#
#-------------------------------------------------

QT       += widgets opengl

TARGET = WebCamCapGui
TEMPLATE = lib

DEFINES += WEBCAMCAPGUI_LIBRARY

SOURCES += webcamcapgui.cpp \
    openglscene.cpp

HEADERS += webcamcapgui.h\
        webcamcapgui_global.h \
    openglscene.h

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

unix {
    target.path = /usr/lib
    INSTALLS += target
}



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/release/ -lWebCamCapPrimitives
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/debug/ -lWebCamCapPrimitives
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/ -lWebCamCapPrimitives

INCLUDEPATH += $$PWD/../WebCamCapPrimitives
DEPENDPATH += $$PWD/../WebCamCapPrimitives
