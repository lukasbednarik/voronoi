QT += testlib
QT -= gui
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app
TARGET = testVoronoi

HEADERS += \
    test1.h

SOURCES += main.cpp \
    test1.cpp

# Voronoi library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lvoronoi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lvoronoi
else:unix: LIBS += -L$$OUT_PWD/../src/ -lvoronoi

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
