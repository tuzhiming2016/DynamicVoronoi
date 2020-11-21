TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR += ./bin
OBJECTS_DIR += ./tmp

SOURCES += \
    dynamicEDT3D.cpp \
#    dynamicEDTOctomap.cpp \
    exampleEDT3D.cpp \
#    exampleEDTOctomap.cpp

HEADERS += \
    dynamicEDT3D/bucketedqueue.h \
    dynamicEDT3D/bucketedqueue.hxx \
    dynamicEDT3D/dynamicEDT3D.h \
    dynamicEDT3D/dynamicEDTOctomap.h \
    dynamicEDT3D/point.h
