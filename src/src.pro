QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CheckForDuplicateFiles
TEMPLATE = app

DEFINES += _DEBUG

SOURCES += \
    main/main.cpp \
    main/mainwindow.cpp \
    core/threadselectfiles.cpp \
    main/itemlistdelegate.cpp \
    main/listwidget.cpp \
    core/widgetUtil.cpp

FORMS += \
    main/mainwindow.ui \
    main/listwidget.ui

HEADERS += \
    main/mainwindow.h \
    core/threadselectfiles.h \
    main/itemlistdelegate.h \
    main/listwidget.h \
    core/widgetUtil.h

TRANSLATIONS += res/translator/translator_cn.ts

RESOURCES += \
    res/resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ComputeHash/release/ -lComputeHash
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ComputeHash/debug/ -lComputeHash
else:unix: LIBS += -L$$OUT_PWD/../ComputeHash/ -lComputeHash

INCLUDEPATH += $$PWD/../ComputeHash
DEPENDPATH += $$PWD/../ComputeHash
