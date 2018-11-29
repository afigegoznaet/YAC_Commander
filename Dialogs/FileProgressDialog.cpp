#include "FileProgressDialog.hpp"
#include <QFileInfoList>
#include "ui_progressDialog.h"
#include "Delegates/FileMoverDelegate.hpp"
#include <QDebug>
#include <set>
ProgressDialog::ProgressDialog(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f), progress(new Ui::ProgressDialog), status(true) {
	progress->setupUi(this);
	progress->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	progress->tableWidget->setColumnCount(3);
	// progress->tableWidget->verticalHeader()->setDefaultSectionSize(
	// this->fontMetrics().height() + 6);

	progress->tableWidget->horizontalHeader()->setStretchLastSection(true);
	progress->tableWidget->horizontalHeader()->setSectionsMovable(true);
	progress->tableWidget->verticalHeader()->setSectionResizeMode(
		QHeaderView::Fixed);

	QStringList m_TableHeader;
	m_TableHeader << "#"
				  << "Name"
				  << "Text";
	progress->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
	progress->progressBar->setMinimum(0);
	progress->progressBar->setMaximum(100);
	connect(this, SIGNAL(dirMoved(int)), this, SLOT(movementResult(int)));

	connect(this, SIGNAL(sendErrMsg(QString)), this, SLOT(errorMsg(QString)),
			Qt::QueuedConnection);
	connect(this, SIGNAL(hideDialogSignal()), this, SLOT(hideDialogSlot()),
			Qt::QueuedConnection);

	QSettings settings;
	auto headerState = settings.value("ProgressColumns").toByteArray();
	progress->tableWidget->horizontalHeader()->restoreState(headerState);
}

ProgressDialog::~ProgressDialog() {
	QSettings settings;
	settings.setValue("ProgressColumns",
					  progress->tableWidget->horizontalHeader()->saveState());
	delete progress;
}

void ProgressDialog::processFileAction(QFileInfoList fileList,
									   const QString &destination,
									   Qt::DropAction action) {

	QFileInfo destDir(destination);
	if (!destDir.isWritable()) {
		QMessageBox::warning(
			this, "Error!",
			"You don't have permissions to write to the destination directory!");
		return;
	}
	if (isHidden())
		show();
	if (!progress->tableWidget->rowCount())
		status = true;

	int initialCount = progress->tableWidget->rowCount();
	int newRow = progress->tableWidget->rowCount();


	for (const auto &fileInfo : fileList) {

		if (!fileInfo.fileName().compare("..", Qt::CaseInsensitive)
			|| !fileInfo.fileName().compare(".", Qt::CaseInsensitive))
			continue;

		if (!fileInfo.absolutePath().compare(destination))
			continue;


		// QString item = "Move " + fileInfo.fileName() + " to " + destination;
		// QString newName = destination + "/" + fileInfo.fileName();

		progress->tableWidget->insertRow(newRow);

		switch (action) {
		case Qt::MoveAction:
			progress->tableWidget->setItem(newRow, 0,
										   new QTableWidgetItem("Move"));
			break;
		case Qt::CopyAction:
			progress->tableWidget->setItem(newRow, 0,
										   new QTableWidgetItem("Copy"));
			break;
		case Qt::LinkAction:
			break;
		default:
			QMessageBox::warning(this, "Error!", "Unsupported action!");
			break;
		}
		progress->tableWidget->setItem(
			newRow, 1, new QTableWidgetItem(fileInfo.absoluteFilePath()));
		progress->tableWidget->setItem(newRow, 2,
									   new QTableWidgetItem(destination));
	}


	if (0 == initialCount)
		QtConcurrent::run(this, &ProgressDialog::processItemsInList);
}

void ProgressDialog::onWrite(uint percentsWritten) {
	if (percentsWritten > static_cast<uint>(progress->progressBar->value()))
		progress->progressBar->setValue(percentsWritten);
	if (100 == percentsWritten)
		progress->progressBar->setValue(0);
}

QMessageBox::StandardButton ProgressDialog::showError(int result) {

	status &= (result & 1);

	static const QString errorCopyMasg = "Copying file failed!\nContinue?";
	static const QString errorMoveMasg = "Moving file failed!\nContinue?";
	static const QString errorCopyEOLMasg = "Copying file failed!";
	static const QString errorMoveEOLMasg = "Moving file failed!";
	QMessageBox::StandardButton reply = QMessageBox::Yes;
	switch (result) {
	case 10: // Move failed
		if (progress->tableWidget->rowCount() > 1)
			reply = QMessageBox::question(this, "Error!!!", errorMoveMasg,
										  QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
			status = true;
		else
			QMessageBox::warning(this, "Error!!!", errorMoveEOLMasg);


		break;
	case 0:
		if (progress->tableWidget->rowCount() > 1)
			reply = QMessageBox::question(this, "Error!!!", errorCopyMasg,
										  QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
			status = true;
		else
			reply = QMessageBox::question(this, "Error!!!", errorCopyEOLMasg);

		break;
	}
	return reply;
}

void ProgressDialog::movementResult(int result) {

	QMutexLocker locker(&moverBlocker);
	if (progress->tableWidget->rowCount())
		progress->tableWidget->removeRow(0);

	auto reply = showError(result);
	if (reply == QMessageBox::Yes) {
		// status = 1;
		QtConcurrent::run(this, &ProgressDialog::processItemsInList);
	}
}


void ProgressDialog::dirMovementResult(int result) {
	QMutexLocker locker(&moverBlocker);
	showError(result);
}

void ProgressDialog::dirParsing(QDir &dir, QString &action, QString &dest,
								QList<QString> &createdDirs) {

	if (!dir.exists(dest)) {
		dir.mkdir(dest);
		createdDirs.append(dest);
	}

	QFileInfoList dirEntries = dir.entryInfoList(
		QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst);

	for (const auto &file : dirEntries) {
		{
			QMutexLocker locker(&moverBlocker);
			// this is a very very very bad hack
			// Need to find a nother way
			if (!status) {
				// qDebug() << "Negative status?";
				return;
			}
		}
		QString destination(dest);
		destination.append("/");
		// qDebug()<<file.fileName();
		// qDebug()<<file.filePath();

		QString source(file.filePath());

		if (file.isDir()) {

			QDir dir(file.filePath());

			destination.append(file.fileName());
			destination.append("/");

			if (!createdDirs.contains(file.absoluteFilePath().append('/')))
				dirParsing(dir, action, destination, createdDirs);
			continue;
		}
		destination.append("/");
		destination.append(file.fileName());
		setWindowTitle(file.fileName());
		FileMoverDelegate mover(source, destination, action, this);
		connect(&mover, SIGNAL(bytesProgress(uint)), this, SLOT(onWrite(uint)));
		connect(&mover, SIGNAL(completed(int)), this,
				SLOT(dirMovementResult(int)));
		connect(this, SIGNAL(setStatus(int)), &mover, SLOT(setStatus(int)),
				Qt::DirectConnection);
		emit setStatus(status);
	}
}

void ProgressDialog::errorMsg(const QString &errorText) {
	QMessageBox::warning(this, "Error", errorText);
	cond.wakeOne();
}

void ProgressDialog::processItemsInList() {
	if (status && progress->tableWidget->rowCount()) {
		// stub.waitForFinished();
		QString source(progress->tableWidget->item(0, 1)->text());
		QString destination(progress->tableWidget->item(0, 2)->text());
		destination.append("/");
		QFileInfo fileInfo(source);
		QString fileName(fileInfo.fileName());
		QString action = progress->tableWidget->item(0, 0)->text();

		if (fileInfo.isDir()) {
			QDir dir(source);
			bool movable = isMovable(source, destination);
			destination.append(fileName);
			// destination.append("/");
			QList<QString> createdDirs;
			/**
			  Too many "if"s, gotta do something about it
			  */
			if (!action.compare("Move", Qt::CaseInsensitive)) {
				if (movable) {
					if (destination.startsWith(source)) {
						emit sendErrMsg("Can not move directory into itself");
						QMutex blocker;
						blocker.lock();
						cond.wait(&blocker); // this releases the mutex
											 // blocker.unlock();
					} else
						dir.rename(source, destination);
				} else {
					dirParsing(dir, action, destination, createdDirs);
					dir.removeRecursively();
				}
			} else
				dirParsing(dir, action, destination, createdDirs);
			emit dirMoved(1);
			return;
		}


		destination.append(fileName);
		FileMoverDelegate mover(source, destination, action, this);

		connect(&mover, SIGNAL(bytesProgress(uint)), this, SLOT(onWrite(uint)));
		connect(&mover, SIGNAL(completed(int)), this, SLOT(movementResult(int)),
				Qt::DirectConnection);
		connect(this, SIGNAL(setStatus(int)), &mover, SLOT(setStatus(int)),
				Qt::DirectConnection);
		emit setStatus(status);

		// delete mover;

	} else {
		setWindowTitle("");
		progress->pauseButton->setText(pauseButtonLabels->at(status));
		emit hideDialogSignal();
	}
}

void ProgressDialog::switchText() {
	progress->pauseButton->setText(pauseButtonLabels->at(status));
}

void ProgressDialog::on_pauseButton_clicked() {

	status = !status;
	switchText();
	emit setStatus(status);
	// qDebug()<<"*************************************";
	// qDebug()<<"status "<<status<<" sent";
}

void ProgressDialog::on_removeButton_clicked() {
	QMutexLocker locker(&moverBlocker);
	auto items = progress->tableWidget->selectedItems();
	std::set<int> rows;
	for (const auto &item : items)
		rows.insert(item->row());

	for (const auto &rowNum : rows) {
		if (0 == rowNum)
			emit setStatus(-1);
		progress->tableWidget->removeRow(rowNum);
	}

	if (!progress->tableWidget->rowCount())
		status = false;
}

void ProgressDialog::on_abortButton_clicked() {
	status = false;
	emit setStatus(-1);
	if (progress->tableWidget->rowCount())
		progress->tableWidget->clear();
	// qDebug()<<"table widget cleared";
}
