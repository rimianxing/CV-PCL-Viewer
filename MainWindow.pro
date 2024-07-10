#-------------------------------------------------
#
# Project created by QtCreator 2020-11-09T15:29:38
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = picture
TEMPLATE = app



SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS  += \
    mainwindow.h
FORMS    += mainwindow.ui

INCLUDEPATH += D:\opencv-4.0.1\build\install\include

LIBS += D:\opencv-4.0.1\build\lib\libopencv_*.a


RESOURCES += \
    res.qrc

RC_ICONS = icon.ico

TRANSLATIONS = \
    other_files/zh_CN.ts \
    other_files/en_US.ts
