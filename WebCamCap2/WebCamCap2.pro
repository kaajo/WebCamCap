#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T11:12:58
#
#-------------------------------------------------

QT       += core gui widgets network concurrent

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = WebCamCap2
TEMPLATE = app

CONFIG += link_pkgconfig

PKGCONFIG += opencv gl glu tbb

SOURCES += main.cpp\
        wccmainwindow.cpp \
    src/localserver.cpp \
    src/markercamera.cpp \
    src/virtualroom.cpp

HEADERS  += wccmainwindow.h \
    src/localserver.h \
    src/markercamera.h \
    src/virtualroom.h

FORMS    += wccmainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapInterfaces/release/ -lWebCamCapInterfaces
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapInterfaces/debug/ -lWebCamCapInterfaces
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapInterfaces/ -lWebCamCapInterfaces

INCLUDEPATH += $$PWD/../WebCamCapInterfaces
DEPENDPATH += $$PWD/../WebCamCapInterfaces

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/release/ -lWebCamCapPrimitives
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/debug/ -lWebCamCapPrimitives
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/ -lWebCamCapPrimitives

INCLUDEPATH += $$PWD/../WebCamCapPrimitives
DEPENDPATH += $$PWD/../WebCamCapPrimitives
