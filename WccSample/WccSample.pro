#-------------------------------------------------
#
# Project created by QtCreator 2015-03-29T10:38:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

TARGET = webcamcap_sample_app
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \

FORMS    += mainwindow.ui \


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/release/ -lWebCamCapPrimitives
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/debug/ -lWebCamCapPrimitives
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapPrimitives/ -lWebCamCapPrimitives

INCLUDEPATH += $$PWD/../WebCamCapPrimitives
DEPENDPATH += $$PWD/../WebCamCapPrimitives

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapAPI/release/ -lWebCamCapAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WebCamCapAPI/debug/ -lWebCamCapAPI
else:unix: LIBS += -L$$OUT_PWD/../WebCamCapAPI/ -lWebCamCapAPI

INCLUDEPATH += $$PWD/../WebCamCapAPI
DEPENDPATH += $$PWD/../WebCamCapAPI

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../WebCamCapAPI/release/libWebCamCapAPI.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../WebCamCapAPI/debug/libWebCamCapAPI.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../WebCamCapAPI/release/WebCamCapAPI.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../WebCamCapAPI/debug/WebCamCapAPI.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../WebCamCapAPI/libWebCamCapAPI.a

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/gcc4/x64/release/ -lfbxsdk

INCLUDEPATH += $$PWD/../../../../../usr/lib/gcc4/x64/release
DEPENDPATH += $$PWD/../../../../../usr/lib/gcc4/x64/release

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/gcc4/x64/release/libfbxsdk.a
