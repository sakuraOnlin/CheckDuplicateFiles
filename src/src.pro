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
    core/backstage.cpp \
    core/widgetUtil.cpp \
    main/listwidget.cpp \
    core/computemodule.cpp \
    main/about.cpp

FORMS += \
    main/mainwindow.ui \
    main/listwidget.ui \
    main/about.ui

HEADERS += \
    main/mainwindow.h \
    core/threadselectfiles.h \
    main/itemlistdelegate.h \
    core/backstage.h \
    core/widgetUtil.h \
    main/listwidget.h \
    core/computemodule.h \
    main/about.h

TRANSLATIONS += res/translator/translator_cn.ts

RESOURCES += \
    res/resource.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ComputeHash/release/ -lComputeHash
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ComputeHash/debug/ -lComputeHash
else:unix:!macx: LIBS += -L$$OUT_PWD/../ComputeHash/ -lComputeHash

INCLUDEPATH += $$PWD/../ComputeHash
DEPENDPATH += $$PWD/../ComputeHash
