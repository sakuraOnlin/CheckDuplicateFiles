QT       -= gui

TARGET = CheckFile
TEMPLATE = lib

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

win32:RC_FILE = CheckFile.rc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
