TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig

PKGCONFIG += opencv

SOURCES += main.cpp

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

