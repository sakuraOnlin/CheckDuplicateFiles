QT       -= gui

TARGET = CheckFile
TEMPLATE = lib

VER_MAJ = 0
VER_MIN = 1
VER_PAT = 19

DEFINES += CHECKFILE_LIBRARY _DEBUG

SOURCES += \
    CheckFile.cpp \
    compute.cpp \
    util/util.cpp \
    util/computefactory.cpp \
    type/crc32.cpp \
    threadreadfile.cpp

HEADERS +=\
    CheckFile.h \
    CheckFile_global.h \
    compute.h \
    util/util.h \
    util/computefactory.h \
    type/crc32.h \
    type/crypto.h \
    threadreadfile.h

TRANSLATIONS += res/translator/CheckFile_cn.ts

unix {
    target.path = /usr/lib
    INSTALLS += target
}
