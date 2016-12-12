#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T19:06:56
#
#-------------------------------------------------

QT       -= gui

TARGET = ComputeHash
TEMPLATE = lib

DEFINES += COMPUTEHASH_LIBRARY

SOURCES += computehash.cpp \
    compute.cpp \
    util/util.cpp \
    factory.cpp

HEADERS += computehash.h\
        computehash_global.h \
    compute.h \
    util/util.h \
    factory.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
