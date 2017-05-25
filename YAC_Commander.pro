#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T23:13:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YAC_Commander
TEMPLATE = app

FORMS    +=     mainwindow.ui \
    progressDialog.ui \
    textviewer.ui

SOURCES += main.cpp\
		mainwindow.cpp \
    tab.cpp \
    customtabwidget.cpp \
    filemover.cpp \
    progressDialog.cpp \
    customtoolbar.cpp \
    newdir.cpp \
    textviewer.cpp

HEADERS  += mainwindow.h \
    tab.h \
    customtabwidget.h \
    filemover.h \
    progressDialog.h \
    customtoolbar.h \
    newdir.h \
    textviewer.h

win32-msvc* {
	QMAKE_LFLAGS_WINDOWS += /STACK:10485760,655360
}

