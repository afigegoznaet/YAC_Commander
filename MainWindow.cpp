#include "MainWindow.hpp"
#include "Dialogs/FileSearchDlg.hpp"
#include "ui_progressDialog.h"
#include "Dialogs/TextViewerDlg.hpp"
#include <QtGlobal>
#include "Widgets/FileTabSelector.hpp"
#include "Views/FileTableView.hpp"
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <Dialogs/NewDirDlg.hpp>
#include <utility>
#include "Views/QHexView.hpp"
#include "Widgets/CommandDropdown.hpp"
#include <QClipboard>
#include <Dialogs/FileProgressDialog.hpp>
#include <Models/OrderedFileSystemModel.hpp>
#include <QKeyEvent>

static char aboutText[] =
	"WUFDIENvbW1hbmRlcgpJbmNlcHRlZCBpbiAyMDE3IGluIENoaXNpbmF1LCBNb2xkb3ZhCmJ5IFJvbWFuIFBvc3RhbmNpdWMKCkZvciBxdWVzdGlvbnMgb3IgZGV2IHNlcnZpY2VzOgpyb21hbi5wb3N0YW5jaXVjQGdtYWlsLmNvbQ==";

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
	/***
	 * Setup Ui
	 * */

	ui->setupUi(this);
	setStatusBar(nullptr);
	/*
		qDebug() << QApplication::font();
		QFont theFont(QApplication::font());
		QFontMetrics metrix(theFont);
		// setFont(theFont);
		auto charHeight = metrix.height();


		ui->leftLabel->setMinimumSize(0, charHeight * 3);
		ui->rightLabel->setMinimumSize(0, charHeight * 3);
	*/
	ui->leftTabWidget->setLabel(ui->leftLabel);
	ui->rightTabWidget->setLabel(ui->rightLabel);

	ui->leftTabWidget->init(ui);
	ui->rightTabWidget->init(ui);

	ui->leftLabel->setText("Left");
	ui->rightLabel->setText("Right");
	// ui->leftLabel->setStyleSheet("border: 1px solid green; background:
	// gray"); ui->rightLabel->setStyleSheet("border: 1px solid green;
	// background: gray");

	movementProgress = new ProgressDialog(this);
	searchDlg = new SearchDialog(this);

	readSettings();
	ui->commandsBox->setMain(this);

	connect(ui->commandsBox, SIGNAL(cdTo(QString)), this, SLOT(cdTo(QString)));
	connect(ui->commandsBox, SIGNAL(focusPreviouslyFocused()), this,
			SLOT(focusPreviouslyFocused()), Qt::QueuedConnection);
	connect(ui->quickBar, SIGNAL(cdTo(QString)), this, SLOT(cdTo(QString)));
	connect(ui->leftTabWidget, &FileTabSelector::focusAquired, [this]() {
		leftTabHasFocus = true;
		ui->rightTabWidget->unfocus();
	});
	connect(ui->rightTabWidget, &FileTabSelector::focusAquired, [this]() {
		leftTabHasFocus = false;
		ui->leftTabWidget->unfocus();
	});

	connect(ui->leftTabWidget, SIGNAL(gotResized(int, int, int)),
			ui->rightTabWidget, SLOT(sectionResized(int, int, int)));
	connect(ui->rightTabWidget, SIGNAL(gotResized(int, int, int)),
			ui->leftTabWidget, SLOT(sectionResized(int, int, int)));
	connect(ui->leftTabWidget, SIGNAL(gotMoved(int, int, int)),
			ui->rightTabWidget, SLOT(sectionMoved(int, int, int)));
	connect(ui->rightTabWidget, SIGNAL(gotMoved(int, int, int)),
			ui->leftTabWidget, SLOT(sectionMoved(int, int, int)));
	connect(ui->leftTabWidget, SIGNAL(currentChanged(int)), ui->leftTabWidget,
			SLOT(indexChanged(int)));
	connect(ui->rightTabWidget, SIGNAL(currentChanged(int)), ui->rightTabWidget,
			SLOT(indexChanged(int)));

	ui->commandsBox->setEditable(true);

	// QTimer::singleShot(200, [&](){emit setFocus(ui->leftTabWidget);});

	connect(ui->leftTabWidget, SIGNAL(setFocusSig(FileTabSelector *)), this,
			SLOT(setFocusSlot(FileTabSelector *)));
	connect(ui->rightTabWidget, SIGNAL(setFocusSig(FileTabSelector *)), this,
			SLOT(setFocusSlot(FileTabSelector *)));

	connect(this, SIGNAL(setFileAction(QFileInfoList, QString, Qt::DropAction)),
			movementProgress,
			SLOT(processFileAction(QFileInfoList, QString, Qt::DropAction)));
	connect(ui->leftTabWidget,
			SIGNAL(setFileAction(QFileInfoList, QString, Qt::DropAction)), this,
			SIGNAL(setFileAction(QFileInfoList, QString, Qt::DropAction)));
	connect(ui->rightTabWidget,
			SIGNAL(setFileAction(QFileInfoList, QString, Qt::DropAction)), this,
			SIGNAL(setFileAction(QFileInfoList, QString, Qt::DropAction)));

	setupActions();

	connect(ui->menuWindow, &QMenu::aboutToShow, [&]() {
		if (focusedSelector()->count() <= 1)
			ui->actionClose_tab->setDisabled(true);
		else
			ui->actionClose_tab->setEnabled(true);
	});

	connect(ui->actionSearch, &QAction::triggered,
			[&] { searchDlg->show(focusedTab()->getDirectory()); });

	ui->menubar->addAction("About", this, SLOT(showAbout()));
	// qDebug()<<QStandardPaths::AppConfigLocation;

	cutActionIndicator.reserve(4);
	cutActionIndicator[0] = 2;
	cutActionIndicator[1] = '\0';
	cutActionIndicator[2] = '\0';
	cutActionIndicator[3] = '\0';
	cutActionPadding.reserve(1044);
	cutActionPadding[0] = 255;
	cutActionPadding[1] = 255;
	cutActionPadding[2] = 255;
	cutActionPadding[3] = 255;
}

MainWindow::~MainWindow() {
	writeSettings();
	delete ui;
}

void MainWindow::showAbout() {
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle("About");
	msgBox.setText(QByteArray::fromBase64(aboutText));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	msgBox.exec();
}


void MainWindow::setFocusSlot(FileTabSelector *tab) {
	if (leftTabHasFocus) {
		if (tab == ui->leftTabWidget)
			return;
	} else if (tab == ui->rightTabWidget)
		return;

	QEvent *event1 =
		new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
	QEvent *event2 =
		new QKeyEvent(QEvent::KeyRelease, Qt::Key_Tab, Qt::NoModifier);
	qApp->postEvent(tab, event1);
	qApp->postEvent(tab, event2);
	// tab->setFocus();
}

void MainWindow::writeSettings() {
	QSettings settings;
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.setValue("defaultEditor", editor);
	settings.setValue("maximized", isMaximized());
	settings.endGroup();

	auto current =
		qobject_cast<FileTableView *>(ui->leftTabWidget->currentWidget());
	settings.setValue("LeftColumns", current->horizontalHeader()->saveState());
	current =
		qobject_cast<FileTableView *>(ui->rightTabWidget->currentWidget());
	settings.setValue("RightColumns", current->horizontalHeader()->saveState());

	settings.setValue("showHidden", showHidden());
}

void MainWindow::readSettings() {
	QSettings settings;

	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(400, 400)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	editor = settings.value("defaultEditor", DEF_EDITOR).toString();
	bool maximized = settings.value("maximized", false).toBool();
	if(maximized)
		setWindowState(Qt::WindowMaximized);
	settings.endGroup();

	ui->action_show_hidden_files->setChecked(
		settings.value("showHidden", true).toBool());

	//ui->actionDefault_editor

	ui->rightTabWidget->readSettings();
	ui->leftTabWidget->readSettings();
	ui->commandsBox->readSettings();


}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	auto key = event->key();
	auto modifier = event->modifiers();
	switch (key) {
	case Qt::Key_F3:
		on_F3_clicked();
		break;
	case Qt::Key_F4:
		if (event->modifiers() == Qt::ShiftModifier) {
			editNewFile();
			break;
		} else {
			on_F4_clicked();
			break;
		}
	case Qt::Key_F5:
		copyFiles();
		break;
	case Qt::Key_F6:
		moveFiles();
		break;
	case Qt::Key_F7:
		if (event->modifiers() == Qt::AltModifier) {
			searchDlg->show(focusedTab()->getDirectory());
			break;
		} else {
			makeDir();
			break;
		}
	case Qt::Key_F8:
	case Qt::Key_Delete:
		deleteFiles();
		break;
	case Qt::Key_F10:
		searchDlg->show(focusedTab()->getDirectory());
		break;
	case Qt::Key_C:
		if (modifier == Qt::ControlModifier) {
			copyToClipboard();
			break;
		}
		[[fallthrough]];
	case Qt::Key_V:
		if (event->modifiers() == Qt::ControlModifier) {
			pasteFromClipboard();
			break;
		}
		[[fallthrough]];
	case Qt::Key_X:
		if (event->modifiers() == Qt::ControlModifier) {
			cutToClipboard();
			break;
		}
		[[fallthrough]];
	default:
		// qDebug()<<event;
		// qDebug()<<key;
		QMainWindow::keyPressEvent(event);
		break;
	case Qt::Key_Tab:
		if (ui->leftTabWidget->currentWidget()->hasFocus())
			ui->rightTabWidget->setFocus();
		else
			ui->leftTabWidget->setFocus();
		break;
	}
}

void MainWindow::copyFiles() {

	QFileInfoList fileList = getSelectedFiles();
	if (fileList.isEmpty())
		return;
	QString destination = getDirInFocus(true);

	if (!getDir(destination, fileList.size(), Qt::CopyAction))
		return;

	QDir dir;
	if (!dir.exists(destination)) {
		QString message =
			"Directory\n" + destination + "\ndoesn't exist.\nCreate it?";
		auto reply = QMessageBox::warning(this, "Warning!", message,
										  QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No)
			return;
		dir.mkdir(destination);
	}

	emit setFileAction(fileList, destination, Qt::CopyAction);
}

void MainWindow::moveFiles() {
	QFileInfoList fileList = getSelectedFiles();
	if (fileList.isEmpty())
		return;
	QString destination = getDirInFocus(true);

	if (!getDir(destination, fileList.size(), Qt::MoveAction))
		return;

	QDir dir;
	if (!dir.exists(destination)) {
		QString message =
			"Directory\n" + destination + "\ndoesn't exist.\nCreate it?";
		auto reply = QMessageBox::warning(this, "Warning!", message,
										  QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No)
			return;
		dir.mkdir(destination);
	}

	emit setFileAction(fileList, destination, Qt::MoveAction);
}


void MainWindow::cutToClipboard() {

	const auto &data =
		focusedTab()->getModel()->mimeData(focusedTab()->getSelectedIndexes());
#ifdef WIN32
	data->setData(
		"application/x-qt-windows-mime;value=\"Preferred DropEffect\"",
		cutActionIndicator); // these cutAction variables are duplicated, need
							 // to do something about it
	data->setData("application/x-qt-windows-mime;value=\"DropDescription\"",
				  cutActionPadding);
#else
	data->setData("application/x-kde-cutselection", "1");
#endif
	// qDebug()<<data->formats();
	QGuiApplication::clipboard()->setMimeData(data);
}

void MainWindow::copyToClipboard() {
	const auto &data =
		focusedTab()->getModel()->mimeData(focusedTab()->getSelectedIndexes());
	QGuiApplication::clipboard()->setMimeData(data);
}

void MainWindow::pasteFromClipboard() {
	const auto &clipboard = QGuiApplication::clipboard();
	auto data = clipboard->mimeData();

	// foreach (auto &url, data->urls()) { qDebug() << url; }

	//	foreach (auto &url, data->formats()) {
	//		// qDebug()<<url;
	//		auto text = data->data(url);
	//		// qDebug()<<text;
	//		//
	// qDebug()<<"*********************************************************";
	//	}

	//	auto status = data->data(
	//		"application/x-qt-windows-mime;value=\"Preferred DropEffect\"");
	// qDebug()<<status;

	bool move = false;
#ifdef WIN32
	move =
		(data->data(
				 "application/x-qt-windows-mime;value=\"Preferred DropEffect\"")
			 .at(0)
		 == 2);
#else
	move = data->data("application/x-kde-cutselection").length() > 0;
#endif
	if (move)
		focusedTab()->getModel()->dropMimeData(data, Qt::MoveAction, 1, 0,
											   QModelIndex());
	else
		focusedTab()->getModel()->dropMimeData(data, Qt::CopyAction, 1, 0,
											   QModelIndex());
}

void MainWindow::deleteFiles() { focusedTab()->deleteSelectedFiles(); }

QString MainWindow::getDirInFocus(bool opposite) {
	auto left =
		qobject_cast<FileTableView *>(ui->leftTabWidget->currentWidget());
	auto right =
		qobject_cast<FileTableView *>(ui->rightTabWidget->currentWidget());
	bool focus = leftTabHasFocus;
	if (opposite)
		focus = !leftTabHasFocus;
	if (focus)
		return left->getDirectory();

	return right->getDirectory();
}

QFileInfoList MainWindow::getSelectedFiles() {
	QFileInfoList list = focusedTab()->getSelectedFiles();
	if (1 == list.size() && 0 == list.begin()->fileName().compare(".."))
		list.clear();
	return list;
}


FileTabSelector *MainWindow::focusedSelector() {

	if (leftTabHasFocus)
		return ui->leftTabWidget;
	return ui->rightTabWidget;
}


FileTableView *MainWindow::focusedTab() {
	return qobject_cast<FileTableView *>(focusedSelector()->currentWidget());
}

void MainWindow::cdTo(const QString &dir) {

	// qDebug()<<"Got it!!!!";
	focusedTab()->cdTo(dir);
}

bool MainWindow::getDir(QString &dirName, int numFiles, Qt::DropAction action) {

	QString message;

	switch (action) {
	case Qt::CopyAction:
		message = "<h3><font color=\"#0000ff\">Copy "
				  + QString::number(numFiles) + " files to:</font><h3>";
		break;
	case Qt::MoveAction:
		message = "<h3><font color=\"#ff0000\">Move "
				  + QString::number(numFiles) + " files to:</font><h3>";
		break;
	default:
		message = "<h3><font color=\"#22b14c\">New directory name:</font><h3>";
	}

	QLabel lbl(this);
	NewDirDlg *dialog = new NewDirDlg(message, dirName, &lbl);
	dialog->adjustSize();
	lbl.show();

	QRect r = geometry();
	int x = r.x() + r.width() / 2;
	int y = r.y() + r.height() / 2;

	dialog->move(x, y);
	int hz = dialog->exec();
	if (hz) {
		dirName = dialog->getName();
		lbl.setText(dirName);
		return true;
	}
	return false;
}


bool MainWindow::getFileName(QString &fileName) {

	QString message =
		"<h3><font color=\"#22b14c\">Enter \
		file name to edit:</font><h3>";

	QLabel lbl(this);
	NewFileDlg *dialog = new NewFileDlg(message, fileName, &lbl);
	dialog->adjustSize();
	lbl.show();

	QRect r = geometry();
	int x = r.x() + r.width() / 2;
	int y = r.y() + r.height() / 2;

	dialog->move(x, y);
	int hz = dialog->exec();
	if (hz) {
		fileName = dialog->getName();
		lbl.setText(fileName);
		return true;
	}
	return false;
}


void MainWindow::makeDir() {

	QDir currDir(getDirInFocus());
	// qDebug()<<currDir;
	QString dirName;
	if (!getDir(dirName, 0, Qt::IgnoreAction))
		return;
	// qDebug()<<dirName;

	bool status = currDir.mkdir(dirName);
	if (!status)
		QMessageBox::critical(this, "Error!",
							  "Unable to create directory " + dirName + " in "
								  + currDir.dirName());
}

void MainWindow::editNewFile() {
	auto path = getDirInFocus();
	QString name = "";
	QFileInfoList fileList = getSelectedFiles();
	for (const auto &file : fileList) {
		if (file.isDir())
			continue;
		name = file.fileName();
	}

	if (getFileName(name)) {
		QStringList args;
		path += '/';
		path += name;
		qDebug() << path;
		args << path;
		QProcess::startDetached(editor, args);
	}
}

void MainWindow::on_F3_clicked() {
	// QMessageBox::information(this,"Info","Not yet implemented");
	QFileInfoList fileList = getSelectedFiles();
	auto textViewer = new TextViewer(this);
	for (const auto &file : fileList)
		textViewer->setDocument(file.absoluteFilePath());
	textViewer->exec();
}

void MainWindow::on_F4_clicked() {

	QFileInfoList fileList = getSelectedFiles();
	for (const auto &file : fileList) {
		if (file.isDir())
			continue;

		QStringList args;
		args << file.absoluteFilePath();
		QProcess::startDetached(editor, args);
	}
}

void MainWindow::on_F5_clicked() { copyFiles(); }

void MainWindow::on_F6_clicked() { moveFiles(); }

void MainWindow::on_F8_clicked() { deleteFiles(); }

void MainWindow::on_F7_clicked() { makeDir(); }

void MainWindow::focusPreviouslyFocused() {
	if (leftTabHasFocus)
		ui->leftTabWidget->currentWidget()->setFocus();
	else
		ui->rightTabWidget->currentWidget()->setFocus();
}

void MainWindow::setupActions() {
	/**
	 * File menu
	 * */

	ui->actionO_pen->setShortcut(QKeySequence::Open);
	connect(ui->actionO_pen, &QAction::triggered, [&]() {
		focusedTab()->on_doubleClicked(focusedTab()->currentIndex());
	});
	// ui->actionOpen_with

	connect(ui->actionView_in_hex_mode, &QAction::triggered, this,
			&MainWindow::on_F3_clicked);

	connect(ui->actionExit, &QAction::triggered,
			[&]() { QApplication::quit(); });

	ui->actionExit->setShortcut(QKeySequence::Quit);
	ui->actionExit->setShortcut(QKeySequence(Qt::ALT + Qt::Key_X));

	/***
	 * End of file menu
	 * */

	/**
	 * Wndow Menu
	 * */
	QAction *addTabAction = ui->actionAdd_tab;
	QAction *duplicateTabAction = ui->actionCopy_tab;
	QAction *closeTabAction = ui->actionClose_tab;
	QAction *copyTabPath = ui->actionCopy_path;

	addTabAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
	duplicateTabAction->setShortcut(
		QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));
	closeTabAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
	copyTabPath->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));

	addAction(addTabAction);
	addAction(duplicateTabAction);
	addAction(closeTabAction);
	addAction(copyTabPath);

	connect(addTabAction, &QAction::triggered,
			[&]() { focusedSelector()->addNewTab(); });
	connect(duplicateTabAction, &QAction::triggered,
			[&]() { focusedSelector()->addNewTab(true); });
	connect(closeTabAction, &QAction::triggered, [&]() {
		auto selector = focusedSelector();
		if (selector->count() <= 1)
			return;
		auto curWidget = selector->currentWidget();
		int curIndex = selector->currentIndex();
		selector->removeTab(curIndex);
		delete curWidget;
		if (curIndex == selector->count())
			selector->setCurrentIndex(curIndex - 1);
		else
			selector->setCurrentIndex(curIndex);
		focusPreviouslyFocused();
	});

	connect(copyTabPath, &QAction::triggered, [&]() {
		QGuiApplication::clipboard()->setText(
			(qobject_cast<FileTableView *>(focusedSelector()->currentWidget()))
				->getDirectory());
	});
	/**
	 * End of Window menu
	 * */
}

void MainWindow::on_action_show_hidden_files_changed() {
	ui->rightTabWidget->showHidden(showHidden());
	ui->leftTabWidget->showHidden(showHidden());
}

void MainWindow::parseParams(int argc, char *argv[]) {
	QString params;
	if (argc > 1) {
		params = argv[1];
		if (QDir(params).exists())
			focusedSelector()->addNewTab(false, params);
	}
}

bool MainWindow::showHidden() {
	return ui->action_show_hidden_files->isChecked();
}

void MainWindow::on_actionDefault_editor_triggered()
{
	auto qlabel = new QLabel("Default editor", this);
	auto lineEdit = new QLineEdit(editor, this);

	QPushButton *createButton = new QPushButton(tr("Ok"));
	createButton->setDefault(true);

	QPushButton *cancelButton = new QPushButton(tr("Cancel"));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(createButton, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);



	QVBoxLayout *lt = new QVBoxLayout;

	lt->addWidget(qlabel);
	lt->addWidget(lineEdit);
	lt->addWidget(buttonBox);

	QDialog edName(this);
	edName.setModal(true);
	edName.setLayout(lt);

	connect(buttonBox, &QDialogButtonBox::accepted, this, [this, lineEdit]{
		qDebug()<<"Line!";
		editor = lineEdit->text();
	});
	connect(buttonBox, SIGNAL(accepted()), &edName, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), &edName, SLOT(reject()));

	edName.adjustSize();
	//qlabel.show();

	QRect r = geometry();
	int x = r.x() + r.width() / 2;
	int y = r.y() + r.height() / 2;

	edName.move(x, y);
	edName.exec();



}
