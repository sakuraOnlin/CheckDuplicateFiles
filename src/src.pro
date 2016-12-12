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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ComputeHash/release/ -lComputeHash
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ComputeHash/debug/ -lComputeHash
else:unix: LIBS += -L$$OUT_PWD/../ComputeHash/ -lComputeHash

INCLUDEPATH += $$PWD/../ComputeHash
DEPENDPATH += $$PWD/../ComputeHash
