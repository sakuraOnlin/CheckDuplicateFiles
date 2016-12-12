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
    type/sha1.cpp \
    util/computefactory.cpp \
    type/crc32.cpp

HEADERS += computehash.h\
        computehash_global.h \
    compute.h \
    util/util.h \
    type/sha1.h \
    util/computefactory.h \
    type/crc32.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
