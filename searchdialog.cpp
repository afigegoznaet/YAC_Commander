#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
	QDialog(parent),
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

	model = new QStringListModel(this);
	ui->listView->setModel(model);

	auto conn = connect(this, SIGNAL(startSearchRecursion(QString,QString)), this,
			SLOT(searchRecursion(QString,QString)), Qt::QueuedConnection);
	qDebug()<<"Conn: "<<conn;
}

SearchDialog::~SearchDialog()
{
	delete ui;
}

void SearchDialog::show(const QString &startDir){
	ui->fileMaskcombo->lineEdit()->setText("*.*");
	ui->startDirCombo->lineEdit()->setText(startDir);
	QDialog::show();
}

QString SearchDialog::updateCombo(CustomDropDown* combo){
	QString text(combo->lineEdit()->text());
	if(combo->findText(text)<0)
		combo->insertItem(0,text);
	return text;
}

void SearchDialog::searchRecursion(QString pattern, QString startDir, searchFlags){

	qDebug()<<pattern <<" "<<startDir;
	QRegularExpression re(pattern);

	if(re.globalMatch(startDir).hasNext())
		addFile(startDir);
	QDir dir(startDir);
	QFileInfoList dirEntries = dir.entryInfoList(QStringList(pattern),
				QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::System | QDir::Hidden);

	qDebug()<<"Starting search recusrion in: "<<startDir;
	foreach (auto file, dirEntries){
		//qDebug()<<"Adding file from:"<<startDir;

		/*qDebug()<<file.isDir();
		qDebug()<<file.absoluteDir();
		qDebug()<<file.absoluteFilePath();*/
		if(file.isDir()){
			//qDebug()<<file.absoluteFilePath()<<" is dir";
			dirQ.push_back(file.absoluteFilePath());
		}
		else
			addFile(file.absoluteFilePath());
	}

	qDebug()<<"Finished search recusrion in: "<<dir.absolutePath();
	if(dirQ.size()){
		//fut = QtConcurrent::run([&](){
			auto nextDir = dirQ.first();
			dirQ.pop_front();
			emit startSearchRecursion(pattern,nextDir);
		//});
	}
}

void SearchDialog::on_searchButton_clicked(){
	QString fileMask = updateCombo(ui->fileMaskcombo);
	QString startDir = updateCombo(ui->startDirCombo);
	searchRecursion(fileMask, startDir);
}

void SearchDialog::addFile(QString& newFile){
	qDebug()<<"Adding new file: "<<newFile;
	int row = model->rowCount();
	//qDebug()<<row;
	if(model->insertRow(row))
		model->setData(model->index(row),newFile);
}
