#-------------------------------------------------
#
# Project created by QtCreator 2014-04-11T21:13:17
#
#-------------------------------------------------

#remove this line to build webcamcap1
requires(0)

QT += core gui opengl testlib concurrent network

LIBS += -lglut

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = MotionCapture
TEMPLATE = app

CONFIG += link_pkgconfig

PKGCONFIG += opencv gl glu tbb

UI_DIR +=  Gui
UI_HEADERS_DIR += Gui

SOURCES += main.cpp\
    capturecamera.cpp \
    line.cpp \
    openglwindow.cpp \
    animation.cpp \
    frame.cpp \
    Gui/addproject.cpp \
    Gui/mainwindow.cpp \
    room.cpp \
    capturethread.cpp \
    Gui/camwidget.cpp \
    Gui/addcamera.cpp \
    pointchecker.cpp \
    matrix.tpp \
    munkres.cpp \
    Gui/aboutwidget.cpp \
    localserver.cpp

HEADERS  += capturecamera.h \
    line.h \
    openglwindow.h \
    animation.h \
    frame.h \
    Gui/addproject.h \
    Gui/mainwindow.h \
    room.h \
    capturethread.h \
    Gui/camwidget.h \
    Gui/addcamera.h \
    pointchecker.h \
    matrix.h \
    munkres.h \
    Gui/aboutwidget.h \
    localserver.h

FORMS    +=  Gui/mainwindow.ui \
    Gui/camwidget.ui \
    Gui/addcamera.ui \
    Gui/addproject.ui \
    Gui/aboutwidget.ui

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

RESOURCES += \
    Pictures/Resources.qrc
