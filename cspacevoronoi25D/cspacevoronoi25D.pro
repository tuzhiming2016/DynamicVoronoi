TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR += ./bin
OBJECTS_DIR += ./tmp

SOURCES += \
    cspacevoronoi.cpp \
    cspacevoronoilayer.cpp \
    dynamicvoronoi.cpp \
    example.cpp

HEADERS += \
    bucketedqueue.h \
    bucketedqueue.hxx \
    cspacevoronoi.h \
    cspacevoronoilayer.h \
    dynamicvoronoi.h \
    MemoryManager.h \
    node.h \
    point.h \
    simplemap.h
