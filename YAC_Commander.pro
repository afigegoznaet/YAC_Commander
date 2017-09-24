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
	textviewer.ui \
    searchdialog.ui

SOURCES += main.cpp\
		mainwindow.cpp \
    tab.cpp \
    customtabwidget.cpp \
    filemover.cpp \
    progressDialog.cpp \
    customtoolbar.cpp \
    newdir.cpp \
    textviewer.cpp \
    HexView/QHexView.cpp \
    dropdown.cpp \
    orderedfilesystemmodel.cpp \
    searchdialog.cpp \
    customdropdown.cpp \
    filefindingsmodel.cpp \
    tableitemdelegate.cpp

HEADERS  += mainwindow.h \
    tab.h \
    customtabwidget.h \
    filemover.h \
    progressDialog.h \
    customtoolbar.h \
    newdir.h \
    textviewer.h \
    HexView/QHexView.h \
    ui_mainwindow.h \
    ui_progressDialog.h \
    dropdown.h \
    orderedfilesystemmodel.h \
    searchdialog.h \
    customdropdown.h \
    filefindingsmodel.h \
    tableitemdelegate.h

win32-msvc* {
	QMAKE_LFLAGS_WINDOWS += /STACK:10485760,655360
}

DISTFILES += \
    HexView/LICENSE

