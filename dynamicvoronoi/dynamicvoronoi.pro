TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR += ./bin
OBJECTS_DIR += ./tmp

SOURCES += \
    dynamicvoronoi.cpp \
    example.cpp

HEADERS += \
    bucketedqueue.h \
    bucketedqueue.hxx \
    dynamicvoronoi.h \
    point.h
