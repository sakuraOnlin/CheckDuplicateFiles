#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T19:06:56
#
#-------------------------------------------------

QT       -= gui

TARGET = ComputeHash
TEMPLATE = lib

DEFINES += COMPUTEHASH_LIBRARY

SOURCES += computehash.cpp

HEADERS += computehash.h\
        computehash_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
