#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T00:05:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HOT2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    findcontour.cpp \
    imageprocessing.cpp \
    config.cpp \
    imageutils.cpp

HEADERS  += mainwindow.h \
    findcontour.h \
    imageprocessing.h \
    config.h \
    imageutils.h

CONFIG += link_pkgconfig
PKGCONFIG += opencv

FORMS    += mainwindow.ui
