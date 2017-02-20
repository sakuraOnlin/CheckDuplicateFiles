QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += \
    main/main.cpp \
    main/mainwindow.cpp \
    core/threadselectfiles.cpp \
    main/itemlistdelegate.cpp \
    core/backstage.cpp \
    core/widgetUtil.cpp \
    main/listwidget.cpp \
    core/computemodule.cpp \
    main/about.cpp \
    main/setting.cpp \
    main/lineeditbuttons.cpp \
    core/configurefile.cpp \
    core/findrepeat.cpp \
    main/help.cpp

FORMS += \
    main/mainwindow.ui \
    main/listwidget.ui \
    main/about.ui \
    main/setting.ui \
    main/help.ui

HEADERS += \
    main/mainwindow.h \
    core/threadselectfiles.h \
    main/itemlistdelegate.h \
    core/backstage.h \
    core/widgetUtil.h \
    main/listwidget.h \
    core/computemodule.h \
    main/about.h \
    main/setting.h \
    main/lineeditbuttons.h \
    core/configurefile.h \
    core/findrepeat.h \
    main/help.h

TRANSLATIONS += res/translator/translator_cn.ts

RESOURCES += \
    res/resource.qrc

win32:RC_FILE = CheckForDuplicateFiles.rc
win32:RC_ICONS = file_search.ico

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CheckFile/release/ -lCheckFile
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CheckFile/debug/ -lCheckFile
else:unix: LIBS += -L$$OUT_PWD/../CheckFile/ -lCheckFile

INCLUDEPATH += $$PWD/../CheckFile
DEPENDPATH += $$PWD/../CheckFile
TARGET = CheckDuplicateFiles
