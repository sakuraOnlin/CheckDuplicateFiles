QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CheckForDuplicateFiles
TEMPLATE = app

SOURCES += \
    main/main.cpp \
    main/mainwindows.cpp

FORMS += \
    main/mainwindows.ui

HEADERS += \
    main/mainwindows.h

