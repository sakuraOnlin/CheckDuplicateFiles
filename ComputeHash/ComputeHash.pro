QT       -= gui

TARGET = ComputeHash
TEMPLATE = lib

VER_MAJ = 0
VER_MIN = 1
VER_PAT = 10

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

TRANSLATIONS += res/translator/computeHash_cn.ts

unix {
    target.path = /usr/lib
    INSTALLS += target
}
