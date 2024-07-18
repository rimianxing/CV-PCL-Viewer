#-------------------------------------------------
#
# Project created by QtCreator 2020-11-09T15:29:38
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = picture
TEMPLATE = app

include(D:/PCL1.12.1/pcl1.12.1.pri)

SOURCES += \
    FileIO.cpp \
    MyCloud.cpp \
    Tools.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS  += \
    FileIO.h \
    MyCloud.h \
    Tools.h \
    mainwindow.h
FORMS    += mainwindow.ui

INCLUDEPATH += D:\opencv4.5.3\msvc_build\install\include
DEPENDPATH += D:\opencv4.5.3\msvc_build\install\include
Debug: {
LIBS += -LD:\opencv4.5.3\msvc_build\install\x64\vc16\lib -lopencv_world453d
}
Release: {
LIBS += -LD:\opencv4.5.3\msvc_build\install\x64\vc16\lib -lopencv_world453
}

RESOURCES += \
    res.qrc

RC_ICONS = icon.ico

TRANSLATIONS += \
    other_files/en_US.ts \
    other_files/zh_CN.ts

QMAKE_PROJECT_DEPTH = 0
