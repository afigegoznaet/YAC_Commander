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
    filemover.cpp \
    progressDialog.cpp \
    newdir.cpp \
    textviewer.cpp \
    HexView/QHexView.cpp \
    orderedfilesystemmodel.cpp \
    searchdialog.cpp \
    filefindingsmodel.cpp \
    tableitemdelegate.cpp \
    CommandDropdown.cpp \
    EditableDropdown.cpp \
    QuickAccessBar.cpp \
    FileTab.cpp \
    FileTabSelector.cpp

HEADERS  += mainwindow.h \
    filemover.h \
    progressDialog.h \
    newdir.h \
    textviewer.h \
    HexView/QHexView.h \
    ui_mainwindow.h \
    ui_progressDialog.h \
    orderedfilesystemmodel.h \
    searchdialog.h \
    filefindingsmodel.h \
    tableitemdelegate.h \
    CommandDropdown.h \
    EditableDropdown.h \
    QuickAccessBar.h \
    FileTab.h \
    FileTabSelector.h

win32-msvc* {
	QMAKE_LFLAGS_WINDOWS += /STACK:10485760,655360
}

DISTFILES += \
    HexView/LICENSE

