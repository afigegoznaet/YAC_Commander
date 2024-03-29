#include "FileSearchDlg.hpp"
#include "MainWindow.hpp"
#include "ui_searchdialog.h"
#include <QLineEdit>
#include <Views/FileTableView.hpp>

SearchDialog::SearchDialog(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f), ui(new Ui::SearchDialog) {
	ui->setupUi(this);
	ui->fileMaskcombo->setParentDlg(this);
	ui->fileMaskcombo->setEditable(true);
	ui->startDirCombo->setParentDlg(this);
	ui->startDirCombo->setEditable(true);
	ui->textSearchCombo->setParentDlg(this);
	ui->textSearchCombo->setEditable(true);

	ui->cmpCombo->addItem("=", 0);
	ui->cmpCombo->addItem(">=", 1);
	ui->cmpCombo->addItem("<=", 2);

	ui->unitCombo->addItem("Bytes", 1);
	ui->unitCombo->addItem("KBytes", 1024);
	ui->unitCombo->addItem("MBytes", 1048576);
	ui->unitCombo->addItem("GBytes", 1073741824);
	// model = new FileFindingsModel(this);

	ui->dateTimeFrom->setTime(QTime());
	ui->dateTimeTo->setDate(QDate::currentDate());
	ui->dateTimeTo->setTime(QTime::currentTime());

	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->searchButton->setFocus();

	setFocusPolicy(Qt::NoFocus);

	model = new QStringListModel(this);
	ui->listView->setModel(model);

	connect(
		this, &SearchDialog::startSearchRecursion, this,
		[&](const QString &pattern, const QString &dir) {
			fut = QtConcurrent::run(this, &SearchDialog::searchRecursion,
									pattern, dir, NAME);
		},
		Qt::QueuedConnection);

	connect(this, SIGNAL(rowsInserted(QModelIndex, int, int)), model,
			SIGNAL(rowsInserted(QModelIndex, int, int)));
	connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this,
			SLOT(on_doubleClicked(QModelIndex)));
	searching = false;

	ui->label->setText("");

	parentWindow = qobject_cast<MainWindow *>(parent);
	searchTime = QTime::currentTime();
	counter = 0;
}

SearchDialog::~SearchDialog() {
	searching = false;
	delete ui;
}

void SearchDialog::show(const QString &startDir) {
	if (!searching)
		ui->startDirCombo->lineEdit()->setText(startDir);
	if (!model->rowCount())
		resetGuiState();

	QDialog::show();
}

void SearchDialog::resetGuiState() {
	ui->fileMaskcombo->lineEdit()->setText("*.*");
	ui->textSearchCombo->lineEdit()->setText("*");

	ui->label->setText("");

	ui->dateCheck->setChecked(false);
	ui->dateTimeFrom->setDate(QDate());
	ui->dateTimeFrom->setTime(QTime());
	ui->dateTimeTo->setDate(QDate::currentDate());
	ui->dateTimeTo->setTime(QTime::currentTime());
	ui->dateTimeFrom->setDisabled(true);
	ui->dateTimeTo->setDisabled(true);

	ui->sizeCheck->setChecked(false);
	ui->cmpCombo->setCurrentIndex(1);
	ui->sizeSpin->setValue(0);
	ui->unitCombo->setCurrentIndex(2);
	ui->cmpCombo->setDisabled(true);
	ui->sizeSpin->setDisabled(true);
	ui->unitCombo->setDisabled(true);

	ui->attributesCheck->setChecked(false);
	ui->dirBox->setChecked(false);
	ui->executableBox->setChecked(false);
	ui->writableBox->setChecked(false);
	ui->readableBox->setChecked(false);
	ui->dirBox->setDisabled(true);
	ui->executableBox->setDisabled(true);
	ui->writableBox->setDisabled(true);
	ui->readableBox->setDisabled(true);
}

QString SearchDialog::updateCombo(EditableDropDown *combo) {
	QString text(combo->lineEdit()->text());
	if (combo->findText(text) < 0)
		combo->insertItem(0, text);
	return text;
}

void SearchDialog::searchRecursion(const QString &pattern,
								   const QString &startDir, searchFlags) {

	model->blockSignals(true);
	ui->label->setMaximumWidth(ui->label->width()
							   + ui->horizontalSpacer->geometry().width());
	ui->label->setText(startDir);
	QDir dir(startDir);

	QFileInfoList dirEntries =
		dir.entryInfoList(QStringList(pattern),
						  QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs
							  | QDir::System | QDir::Hidden | QDir::NoSymLinks);

	for (const auto &file : dirEntries) {
		// qDebug()<<"Test: "<<file.isFile()<<" || "<<ui->dirBox->isChecked();
		if (file.isFile() || ui->dirBox->isChecked())
			validateFile(file);
		if (file.isDir())
			dirQ.push_back(file.absoluteFilePath());
	}
	int lastRow = model->rowCount() - 1;

	if (lastRow - firstRow > 10
		|| searchTime.secsTo(QTime::currentTime()) > 10) {
		searchTime = QTime::currentTime();
		model->blockSignals(false);
		emit rowsInserted(model->index(0).parent(), firstRow, lastRow);
		firstRow = model->rowCount();
	}

	dirListLocker.lock();
	if (!dirQ.empty() && searching) {
		auto nextDir = std::move(dirQ.first());
		dirQ.pop_front();
		dirListLocker.unlock();
		emit startSearchRecursion(pattern, nextDir);

	} else {
		dirListLocker.unlock();
		model->blockSignals(false);
		emit rowsInserted(model->index(0).parent(), firstRow, lastRow);
		ui->searchButton->setText("Search");
		ui->label->setText(" ");
		searching = false;
	}
}

void SearchDialog::on_searchButton_clicked() {
	// qDebug()<<"GUI tread ID: "<<QThread::currentThreadId();
	if (searching) {
		ui->searchButton->setText("Search");
		searching = false;
		return;
	}
	attrs.togglesFlags = 0;
	firstRow = 0;
	searching = true;
	QString fileMask = updateCombo(ui->fileMaskcombo);
	QString startDir = updateCombo(ui->startDirCombo);
	QString textPattern = updateCombo(ui->textSearchCombo);
	if (textPattern.isEmpty() || textPattern.compare("*")) {
		attrs.togglesFlags = TextPattern;
		attrs.pattern = textPattern;
	}
	if (ui->dateCheck->isChecked()) {
		attrs.togglesFlags |= Date;
		attrs.startDate = ui->dateTimeFrom->dateTime();
		attrs.endDate = ui->dateTimeTo->dateTime();
	}
	if (ui->sizeCheck->isChecked()) {
		attrs.togglesFlags |= Size;
		attrs.op = static_cast<SizeOp>(ui->cmpCombo->currentData().toInt());
		attrs.size = quint64(ui->sizeSpin->value());
		int mod = 10 * ui->unitCombo->currentData().toInt();
		attrs.size *= pow(2, mod);
	}
	if (ui->attributesCheck->isChecked()) {
		attrs.togglesFlags |= Attributes;
        attrs.attrFlags = (ui->executableBox->isChecked() ? X_EC : NONE)
                          | (ui->writableBox->isChecked() ? WRITE : NONE)
                          | (ui->readableBox->isChecked() ? READ : NONE)
                          | (ui->dirBox->isChecked() ? DIRECTORY : NONE);
	}
	model->setStringList(QStringList{});
	ui->searchButton->setText("Stop search");
	fut = QtConcurrent::run(this, &SearchDialog::searchRecursion, fileMask,
							startDir, NAME);
}

void SearchDialog::addFile(const QString &newFile) {
	// qDebug()<<"Add tread ID: "<<QThread::currentThreadId();
	// qDebug()<<"Adding new file: "<<newFile;
	int row = model->rowCount();
	// qDebug()<<row;
	if (model->insertRow(row))
		model->setData(model->index(row), newFile);
}

void SearchDialog::on_doubleClicked(const QModelIndex &index) {
	QString info = model->data(index, 0).toString();
	parentWindow->focusedTab()->goToFile(info);
	hide();
}

void SearchDialog::validateFile(const QFileInfo &theFile) {
	if (!searching)
		return;
	if (attrs.togglesFlags & TextPattern) {
		auto file = new QFile(theFile.absoluteFilePath());
		if (!file->open(QIODevice::ReadOnly))
			return;
		QString		line;
		QTextStream in(file);
		bool		caseSens = ui->caseCheckBox->isChecked();
		bool		regEx = ui->regExpCheckBox->isChecked();
		auto		searchCond =
			regEx ? std::function<bool(const QString &, const QString &, bool)>(
				[](const QString &line, const QString &pattern, bool) {
					QRegularExpression rx(pattern);
					return rx.globalMatch(line).hasNext();
				})
				  : std::function<bool(const QString &, const QString &, bool)>(
					  [](const QString &line, const QString &pattern,
						 bool caseSens) {
						  return line.contains(pattern,
											   Qt::CaseSensitivity(caseSens));
					  });

		bool found = false;
		while (!in.atEnd()) {
			if (!searching)
				break;
			line = in.readLine();
			if (searchCond(line, attrs.pattern, caseSens)) {
				found = true;
				break;
			}
		}
		file->close();
		if (!found)
			return;
	}

	if (attrs.togglesFlags & Date) {

		auto startDate = ui->dateTimeFrom->dateTime();
		auto endDate = ui->dateTimeTo->dateTime();

		if (theFile.birthTime() < startDate || theFile.birthTime() > endDate)
			return;
	}

	if (attrs.togglesFlags & Size) {

		qint64 searchedSize{qint64(ui->sizeSpin->value())};
		auto   operation = ui->cmpCombo->currentData().toInt();
		qint64 multiplier = ui->unitCombo->currentData().toLongLong();

		//	ui->cmpCombo->addItem("=",0);
		// ui->cmpCombo->addItem(">=",1);
		// ui->cmpCombo->addItem("<=",2);

		switch (operation) {
		case 0:
			if (theFile.size() * multiplier != searchedSize)
				return;
			break;
		case 1:
			if (theFile.size() * multiplier < searchedSize)
				return;
			break;
		case 2:
			if (theFile.size() * multiplier > searchedSize)
				return;
			break;
		}
	}

	if (attrs.togglesFlags & Attributes) {
		/*qDebug()<<"Flags test:
		"<<theFile.permission(QFileDevice::Permission(attrs.attrFlags));
		qDebug()<<theFile.isDir();
		qDebug()<<theFile.isExecutable();
		qDebug()<<theFile.isReadable();
		qDebug()<<theFile.isWritable();*/
        if (attrs.attrFlags & DIRECTORY) {
			if (!theFile.isDir())
				return;
			QRegularExpression re(ui->fileMaskcombo->lineEdit()->text());
			if (!re.globalMatch(theFile.fileName()).hasNext())
				return;
		}

        if (!(attrs.attrFlags & DIRECTORY)
			&& !theFile.permission(QFileDevice::Permission(
                attrs.attrFlags & (X_EC | READ | WRITE))))
			return;
	}

	addFile(theFile.absoluteFilePath());
}
/*
void SearchDialog::paintEvent(QPaintEvent *event){
	if(searching)
		qDebug()<<"Paint tread ID: "<<QThread::currentThreadId();
	//qDebug()<<event;
	//addBlocker.lock();
	QDialog::paintEvent(event);
	//addBlocker.unlock();
}
*/

void SearchDialog::on_clearButton_clicked() {
	searching = true;
	on_searchButton_clicked();
	model->setStringList(QStringList{});
	resetGuiState();
}

void SearchDialog::on_dateCheck_toggled(bool checked) {
	if (checked) {
		ui->dateTimeFrom->setEnabled(true);
		ui->dateTimeTo->setEnabled(true);
		return;
	}
	ui->dateTimeFrom->setDisabled(true);
	ui->dateTimeTo->setDisabled(true);
}

void SearchDialog::on_sizeCheck_toggled(bool checked) {
	if (checked) {
		ui->cmpCombo->setEnabled(true);
		ui->sizeSpin->setEnabled(true);
		ui->unitCombo->setEnabled(true);
		return;
	}
	ui->cmpCombo->setDisabled(true);
	ui->sizeSpin->setDisabled(true);
	ui->unitCombo->setDisabled(true);
}

void SearchDialog::on_attributesCheck_toggled(bool checked) {
	if (checked) {
		ui->dirBox->setEnabled(true);
		ui->executableBox->setEnabled(true);
		ui->writableBox->setEnabled(true);
		ui->readableBox->setEnabled(true);
		return;
	}
	ui->dirBox->setDisabled(true);
	ui->executableBox->setDisabled(true);
	ui->writableBox->setDisabled(true);
	ui->readableBox->setDisabled(true);
}


void SearchDialog::on_caseCheckBox_stateChanged(int checkState) {
	if (searching) {
		ui->caseCheckBox->setChecked(!checkState);
		return;
	}
	if (Qt::Checked == checkState)
		ui->regExpCheckBox->setChecked(false);
}

void SearchDialog::on_regExpCheckBox_stateChanged(int checkState) {
	if (searching) {
		ui->regExpCheckBox->setChecked(!checkState);
		return;
	}
	if (Qt::Checked == checkState)
		ui->caseCheckBox->setChecked(false);
}
