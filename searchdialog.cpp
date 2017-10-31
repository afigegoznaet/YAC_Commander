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

	connect(this, SIGNAL(startSearchRecursion(QString,QString)), this,
			SLOT(searchRecursion(QString,QString)), Qt::QueuedConnection);
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

	//qDebug()<<pattern <<" "<<startDir;
	QRegularExpression re(pattern);
	QDir dir(startDir);
	if(re.globalMatch(dir.dirName()).hasNext())
		addFile(dir.absolutePath());

	QFileInfoList dirEntries = dir.entryInfoList(QStringList(pattern),
				QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::System | QDir::Hidden);

	//qDebug()<<"Starting search recusrion in: "<<startDir;
	foreach (auto file, dirEntries){
		//qDebug()<<"Adding file from:"<<startDir;

		/*qDebug()<<file.isDir();
		qDebug()<<file.absoluteDir();
		qDebug()<<file.absoluteFilePath();*/
		if(file.isFile()){
			addFile(file.absoluteFilePath());
		}else if(file.isDir()){
			dirQ.push_back(file.absoluteFilePath());
			/*qDebug()<<"file is dir: "<<file.fileName();
			qDebug()<<file.baseName();
			qDebug()<<file.isBundle();
			qDebug()<<file.isDir();
			qDebug()<<file.isFile();
			qDebug()<<file.isExecutable();*/
		}

	}

	//qDebug()<<"Finished search recusrion in: "<<dir.absolutePath();
	if(dirQ.size()){
		auto nextDir = dirQ.first();
		dirQ.pop_front();
		emit startSearchRecursion(pattern,nextDir);
	}else{
		ui->searchButton->setText("Search");
		searching = false;
	}
}

void SearchDialog::on_searchButton_clicked(){
	if(searching){
		ui->searchButton->setText("Search");
		searching = false;
		return;
	}
	QString fileMask = updateCombo(ui->fileMaskcombo);
	QString startDir = updateCombo(ui->startDirCombo);
	model->setStringList( QStringList{} );
	ui->searchButton->setText("Stop search");
	searchRecursion(fileMask, startDir);
}

void SearchDialog::addFile(QString& newFile){
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
