#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T19:06:56
#
#-------------------------------------------------

QT       -= gui

TARGET = ComputeHash
TEMPLATE = lib

DEFINES += COMPUTEHASH_LIBRARY _DEBUG

SOURCES += computehash.cpp \
    compute.cpp \
    util/util.cpp \
    util/computefactory.cpp \
    type/crc32.cpp \
    threadreadfile.cpp

HEADERS += computehash.h\
        computehash_global.h \
    compute.h \
    util/util.h \
    util/computefactory.h \
    type/crc32.h \
    type/crypto.h \
    threadreadfile.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
