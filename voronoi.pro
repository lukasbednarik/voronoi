TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/beachline.cpp \
    src/geometry.cpp \
    src/main.cpp \
    src/voronoi.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    src/beachline.h \
    src/edge.h \
    src/event.h \
    src/geometry.h \
    src/make_unique.h \
    src/point.h \
    src/voronoi.h

