#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T11:12:58
#
#-------------------------------------------------

QT       += core gui widgets network concurrent opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = WebCamCap2
TEMPLATE = app

CONFIG += link_pkgconfig

PKGCONFIG += opencv gl glu tbb

SOURCES += main.cpp\
    gui/wccmainwindow.cpp \
    src/localserver.cpp \
    src/markercamera.cpp \
    src/virtualroom.cpp \
    gui/addcamera.cpp \
    gui/projectwizard.cpp \
    src/polygoncameratopology.cpp \
    src/munkres.cpp \
    src/pointchecker.cpp \
    src/matrix.tpp

HEADERS  += gui/wccmainwindow.h \
    src/localserver.h \
    src/markercamera.h \
    src/virtualroom.h \
    gui/addcamera.h \
    gui/projectwizard.h \
    src/polygoncameratopology.h \
    src/munkres.h \
    src/pointchecker.h \
    src/matrix.h \
    src/std_2d_vector.h

FORMS    += gui/wccmainwindow.ui \
    gui/addcamera.ui \
    gui/projectwizard.ui

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapGui/release/ -lWebCamCapGui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapGui/debug/ -lWebCamCapGui
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapGui/ -lWebCamCapGui

INCLUDEPATH += $$PWD/../WebCamCapGui
DEPENDPATH += $$PWD/../WebCamCapGui

RESOURCES += \
    WebCamCap2.qrc
