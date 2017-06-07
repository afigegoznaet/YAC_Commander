#include "tab.h"

#define IN 1
#define OUT 0
#define cout qDebug()

TabbedListView::TabbedListView(QDir directory, QWidget *parent) :
	QTableView(parent),
	directory(directory.absolutePath())
	{

	//setLayoutMode(QListView::Batched);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setTabKeyNavigation(false);


	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setSectionsMovable(true);
	horizontalHeader()->setSectionsClickable(true);
	horizontalHeader()->setSortIndicatorShown(true);

	verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(fontMetrics().height()+4);

	model = new QFileSystemModel(this);
	model->setRootPath(this->directory);
	model->setFilter(QDir::AllEntries | QDir::NoDot | QDir::System);

	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setModel(model);
	setRootIndex(model->index(model->rootPath()));
	verticalHeader()->setVisible(false);

	connect(model,SIGNAL(directoryLoaded(QString)),this,SLOT(setCurrentSelection(QString)));
	qDebug()<<directory.absolutePath();
}


void TabbedListView::on_doubleClicked(const QModelIndex &index){
	QFileInfo info=model->fileInfo(index);
	if(info.isDir()){
		if(info.fileName().compare(".."))
			chDir(index, IN);
		else
			chDir(index, OUT);
	}else{
		QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
	}
}

void TabbedListView::chDir(const QModelIndex &index, bool in_out){
	//qDebug()<<"Dir at input: "<<model->rootPath();
	if(in_out == IN){
		directory="..";//clever selection
		QDir parentDir(model->fileInfo(index).absoluteFilePath());
		model->setRootPath(parentDir.absolutePath());
		setRootIndex(model->index(model->rootPath()));
	}else{
		QDir parentDir(model->rootPath());
		if(parentDir.isRoot())
			return;
		directory=parentDir.dirName();
		parentDir.cdUp();
		model->setRootPath(parentDir.absolutePath());
		setRootIndex(model->index(model->rootPath()));
	}

	emit dirChanged(model->rootPath(), this->index);

}

void TabbedListView::keyPressEvent(QKeyEvent *event){
	//qDebug()<<event->key();
	QModelIndex index;
	//QModelIndexList items = selectionModel()->selectedIndexes();
	if(selectedIndexes().size()>0)
		index = currentIndex();
	else
		index = rootIndex().child(0,0);
	auto key = event->key();
	switch (key) {
	case Qt::Key_Return:
		on_doubleClicked(index);
		break;
	case Qt::Key_Backspace:
		chDir(index, OUT);
		break;
	default:
		//break;
		QAbstractItemView::keyPressEvent(event);
		break;
	}
	qDebug()<<model->fileInfo(currentIndex()).absoluteFilePath();
}

void TabbedListView::init(){
	connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_doubleClicked(QModelIndex)));
	horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	auto hz = connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));
	qDebug()<<hz;
}

void TabbedListView::setCurrentSelection(QString sel){
	//qDebug()<<"Sel: "<<sel<<" |";
	//QString dotdot("..");
	int rows = model->rowCount(rootIndex());
	QModelIndex ind;
	for(int i=0;i<rows;i++){
		ind = rootIndex().child(i,0);
		//qDebug()<< "Index: "<<i<<" filename: " << model->fileInfo(ind).fileName() << " directory: "<<directory;
		if(!directory.compare(model->fileInfo(ind).fileName()))
			break;
	}

	if(ind.isValid())
		setCurrentIndex(ind);

	selectionModel()->select(currentIndex(), QItemSelectionModel::Select);
	//qDebug()<<model->fileInfo(currentIndex()).fileName();
}

void TabbedListView::headerClicked(int section){

	Qt::SortOrder order = Qt::AscendingOrder;
	if(section == horizontalHeader()->sortIndicatorSection())
		if(Qt::AscendingOrder == horizontalHeader()->sortIndicatorOrder())
			order = Qt::DescendingOrder;


	model->sort(section, order);
	//setModel(model);
}

void TabbedListView::focusInEvent(QFocusEvent *event){
	//cout<<"Focus in! "<<event->gotFocus();
	QWidget::focusInEvent(event);
	emit focusEvent(true);
}

void TabbedListView::focusOutEvent(QFocusEvent *event){
	//cout<<"Focus out! "<<event->gotFocus();
	QWidget::focusOutEvent(event);
	emit focusEvent(false);
}

QFileInfoList TabbedListView::getSelectedFiles(){
	QFileInfoList selectedFiles;
	QModelIndexList items = selectionModel()->selectedRows();
	foreach (auto fileIndex, items) {
		selectedFiles.append(model->fileInfo(fileIndex));
	}
	return selectedFiles;
}

void TabbedListView::cdTo(const QString &dir){
	qDebug()<<"Cd event!!! "<<dir;
	model->setRootPath(dir);
	setRootIndex(model->index(model->rootPath()));
}

void TabbedListView::mousePressEvent(QMouseEvent *event){
	if(event->button() != Qt::RightButton){
		QTableView::mousePressEvent(event);
		return;
	}

	qDebug()<<"Right mouse!";
}
