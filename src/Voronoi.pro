CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

TEMPLATE = lib
TARGET = voronoi

SOURCES += \
    beachline.cpp \
    geometry.cpp \
    voronoi.cpp

HEADERS += \
    beachline.h \
    edge.h \
    event.h \
    geometry.h \
    make_unique.h \
    point.h \
    voronoi.h





