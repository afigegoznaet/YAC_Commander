#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T23:13:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets KIOCore KIOFileWidgets KIOWidgets KNTLM

TARGET = yc
TEMPLATE = app

FORMS    +=     mainwindow.ui \
	progressDialog.ui \
	textviewer.ui \
	searchdialog.ui

SOURCES += main.cpp\
	Widgets/CommandDropdown.cpp \
	Widgets/EditableDropdown.cpp \
	Widgets/QuickAccessBar.cpp \
	Dialogs/TextViewerDlg.cpp \
	Models/FileFindingsModel.cpp \
	Models/OrderedFileSystemModel.cpp \
	Delegates/FileMoverDelegate.cpp \
	Views/FileTabView.cpp \
	Widgets/FileTabSelector.cpp \
	MainWindow.cpp \
	Dialogs/NewDirDlg.cpp \
	Dialogs/FileProgressDialog.cpp \
	Dialogs/FileSearchDlg.cpp \
	Delegates/TableItemDelegate.cpp \
	Views/QHexView.cpp \
	Menus/ItemContextMenu.cpp

HEADERS  += \
	Delegates/FileMoverDelegate.hpp \
	Delegates/TableItemDelegate.hpp \
	Dialogs/FileProgressDialog.hpp \
	Dialogs/FileSearchDlg.hpp \
	Dialogs/NewDirDlg.hpp \
	Dialogs/TextViewerDlg.hpp \
	Models/FileFindingsModel.hpp \
	Models/OrderedFileSystemModel.hpp \
	Widgets/CommandDropdown.hpp \
	Widgets/EditableDropdown.hpp \
	Widgets/FileTabSelector.hpp \
	Widgets/QuickAccessBar.hpp \
	mainwindow.hpp \
	Views/QHexView.hpp \
	Views/FileTabView.hpp \
	Menus/ItemContextMenu.hpp

win32-msvc* {
	QMAKE_LFLAGS_WINDOWS += /STACK:10485760,655360
}

linux {
	target.path = /usr/local/bin
	desktop.path = /usr/share/applications
	desktop.files += yc.desktop
	INSTALLS += target desktop
	INCLUDEPATH += /usr/include/KDE

}

DISTFILES += \
	HexView/LICENSE

