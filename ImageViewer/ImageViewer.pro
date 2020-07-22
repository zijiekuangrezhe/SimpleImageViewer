#-------------------------------------------------
#
# Project created by QtCreator 2014-08-24T12:37:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = app

win32:RC_ICONS += 73.ico

SOURCES += main.cpp\
        imageviewer.cpp \
    about.cpp

HEADERS  += imageviewer.h \
    about.h

FORMS    += imageviewer.ui \
    about.ui
