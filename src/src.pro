QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CheckForDuplicateFiles
TEMPLATE = app

SOURCES += \
    main/main.cpp \
    main/mainwindow.cpp

FORMS += \
    main/mainwindow.ui

HEADERS += \
    main/mainwindow.h

TRANSLATIONS += res/translator/translator_cn.ts

RESOURCES += \
    res/resource.qrc

