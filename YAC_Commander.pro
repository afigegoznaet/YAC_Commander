#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T23:13:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YAC_Commander
TEMPLATE = app

FORMS    +=     dialog.ui mainwindow.ui

SOURCES += main.cpp\
    dialog.cpp \
		mainwindow.cpp \
    tab.cpp \
    customtabwidget.cpp \
    statuschecker.cpp

HEADERS  += mainwindow.h \
    tab.h \
    customtabwidget.h \
    dialog.h \
    statuschecker.h
