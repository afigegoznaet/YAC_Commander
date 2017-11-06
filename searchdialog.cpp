#include "searchdialog.h"
#include "mainwindow.h"

SearchDialog::SearchDialog(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::SearchDialog)
{
	ui->setupUi(this);
	ui->fileMaskcombo->setParentDlg(this);
	ui->fileMaskcombo->setEditable(true);
	ui->startDirCombo->setParentDlg(this);
	ui->startDirCombo->setEditable(true);

	//model = new FileFindingsModel(this);

	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	setFocusPolicy(Qt::NoFocus);

	model = new QStringListModel(this);
	ui->listView->setModel(model);

	connect(this, &SearchDialog::startSearchRecursion, this, [&](QString pattern,QString dir){
		fut = QtConcurrent::run(this, &SearchDialog::searchRecursion,pattern, dir, NAME);
	}, Qt::QueuedConnection);

	connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), model, SIGNAL(rowsInserted(QModelIndex,int,int)));
	connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_doubleClicked(QModelIndex)));
	searching = false;

	parentWindow = qobject_cast<MainWindow*>(parent);
}

SearchDialog::~SearchDialog()
{
	delete ui;
}

void SearchDialog::show(const QString &startDir){
	if(!searching){
		ui->fileMaskcombo->lineEdit()->setText("*.*");
		ui->startDirCombo->lineEdit()->setText(startDir);
	}
	QDialog::show();
}

QString SearchDialog::updateCombo(CustomDropDown* combo){
	QString text(combo->lineEdit()->text());
	if(combo->findText(text)<0)
		combo->insertItem(0,text);
	return text;
}

void SearchDialog::searchRecursion(QString pattern, QString startDir, searchFlags){

	model->blockSignals(true);
	QRegularExpression re(pattern);
	QDir dir(startDir);
	if(re.globalMatch(dir.dirName()).hasNext())
		addFile(dir.absolutePath());

	QFileInfoList dirEntries = dir.entryInfoList(QStringList(pattern),
				QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::System | QDir::Hidden);

	foreach (auto file, dirEntries){
		if(file.isFile()){
			addFile(file.absoluteFilePath());
		}else if(file.isDir()){
			dirQ.push_back(file.absoluteFilePath());
		}

	}

	int lastRow = model->rowCount()-1;
	if(lastRow-firstRow > 50){
		model->blockSignals(false);
		emit rowsInserted(model->index(0).parent(), firstRow, lastRow);
		firstRow = model->rowCount();
	}

	if(dirQ.size() && searching){
		auto nextDir = dirQ.first();
		dirQ.pop_front();
		emit startSearchRecursion(pattern,nextDir);
	}else{
		model->blockSignals(false);
		emit rowsInserted(model->index(0).parent(), firstRow, lastRow);
		ui->searchButton->setText("Search");
		searching = false;
	}

}

void SearchDialog::on_searchButton_clicked(){
	qDebug()<<"GUI tread ID: "<<QThread::currentThreadId();
	if(searching){
		ui->searchButton->setText("Search");
		searching = false;
		return;
	}
	firstRow = 0;
	searching = true;
	QString fileMask = updateCombo(ui->fileMaskcombo);
	QString startDir = updateCombo(ui->startDirCombo);
	model->setStringList( QStringList{} );
	ui->searchButton->setText("Stop search");
	fut = QtConcurrent::run(this, &SearchDialog::searchRecursion,fileMask, startDir, NAME);
}

void SearchDialog::addFile(const QString& newFile){
	//qDebug()<<"Add tread ID: "<<QThread::currentThreadId();
	//qDebug()<<"Adding new file: "<<newFile;
	int row = model->rowCount();
	//qDebug()<<row;
	if(model->insertRow(row))
		model->setData(model->index(row),newFile);

}

void SearchDialog::on_doubleClicked(const QModelIndex &index){
	QString info=model->data(index, 0).toString();
	parentWindow->getFocusedTab()->goToFile(info);
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
