#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T11:43:53
#
#-------------------------------------------------


TARGET = WebCamCapInterfaces
TEMPLATE = lib

DEFINES += WEBCAMCAPINTERFACES_LIBRARY

SOURCES += webcamcapinterfaces.cpp \
    ivirtualroom.cpp \
    icamera.cpp

HEADERS += webcamcapinterfaces.h\
        webcamcapinterfaces_global.h \
    ivirtualroom.h \
    icamera.h

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

unix {
    target.path = /usr/lib/
    INSTALLS += target
}

CONFIG += link_pkgconfig

PKGCONFIG += opencv gl glu

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/release/ -lWebCamCapPrimitives
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/debug/ -lWebCamCapPrimitives
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/ -lWebCamCapPrimitives

INCLUDEPATH += $$PWD/../WebCamCapPrimitives
DEPENDPATH += $$PWD/../WebCamCapPrimitives



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapGui/release/ -lWebCamCapGui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapGui/debug/ -lWebCamCapGui
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapGui/ -lWebCamCapGui

INCLUDEPATH += $$PWD/../WebCamCapGui
DEPENDPATH += $$PWD/../WebCamCapGui
