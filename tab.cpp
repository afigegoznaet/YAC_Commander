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

	model = new OrderedFileSystemModel(this);
	auto fModel = new QFileSystemModel(this);
	model->setSourceModel(fModel);
	model->setRootPath(this->directory);
	model->setFilter(QDir::AllEntries | QDir::NoDot | QDir::System);

	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setModel(model);
	model->setFilterRegExp("");
	setRootIndex(model->getRootIndex());
	verticalHeader()->setVisible(false);

	connect(fModel, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(rowsAboutToBeInserted(QModelIndex,int,int)));
	connect(fModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));
	connect(fModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInserted(QModelIndex,int,int)));
	connect(fModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemoved(QModelIndex,int,int)));
	qDebug()<<directory.absolutePath();

}


void TabbedListView::on_doubleClicked(const QModelIndex &index){
	QFileInfo info=model->fileInfo(index);//mapped
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

	}else{
		QDir parentDir(model->rootPath());
		if(parentDir.isRoot())
			return;
		directory=parentDir.dirName();
		parentDir.cdUp();
		model->setRootPath(parentDir.absolutePath());
	}
	setRootIndex(model->getRootIndex());
	emit dirChanged(model->rootPath(), this->index);

}

void TabbedListView::keyPressEvent(QKeyEvent *event){
	//qDebug()<<event->key();
	QString filter;
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
	case Qt::Key_Plus:
		setSelection(PLUS);
		break;
	case Qt::Key_Minus:
		setSelection(MINUS);
		break;
	case Qt::Key_Asterisk:
		setSelection(ASTERISK);
		break;
	default:
		//break;
		//qDebug()<<"key: "<<key;
		//qDebug()<<"event: "<<event;
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

void TabbedListView::setCurrentSelection(QString){
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
	setRootIndex(model->getRootIndex());
	emit dirChanged(dir, this->index);
}

void TabbedListView::mousePressEvent(QMouseEvent *event){
	if(event->button() != Qt::RightButton){
		QTableView::mousePressEvent(event);
		return;
	}

	qDebug()<<"Right mouse!";
}

void TabbedListView::queryDialog(QString& filter, Action act){
	bool ok;
	QString label;
	switch (act) {
	case PLUS:
		label = "Select";
		break;
	case MINUS:
		label = "Deselect";
		break;
	default:
		return;
	}
	filter = QInputDialog::getText(this, "Filter selection",
										 label, QLineEdit::Normal,
										 "*.*", &ok);

}

void TabbedListView::setSelection(Action act){
	QString filter("");
	int rowCount = model->rowCount(rootIndex());
	int columnCount = model->columnCount(rootIndex());
	QItemSelectionModel::SelectionFlag selectionType;
	switch(act){
	case PLUS:
		selectionType = QItemSelectionModel::Select;
		queryDialog(filter, PLUS);
		break;
	case MINUS:
		selectionType = QItemSelectionModel::Deselect;
		queryDialog(filter, MINUS);
		break;
	default:
		selectionType = QItemSelectionModel::Toggle;
		break;
	}

	QRegExp reg(filter, Qt::CaseSensitive, QRegExp::Wildcard);
	for(int i = 0; i<rowCount;i++){
		auto ind = rootIndex().child(i,0);

		if(!model->fileInfo(ind).fileName().compare(".."))
			continue;

		if(filter.isEmpty() || model->fileInfo(ind).fileName().contains(reg)){
			selectionModel()->select(ind, selectionType );
			for(int j=1;j<columnCount;j++)
			selectionModel()->select(rootIndex().child(i,j), selectionType);
		}
	}
}


void TabbedListView::rowsAboutToBeRemoved(const QModelIndex &parent, int first, int){
	prevSelection = parent.child(first, 0);
}

void TabbedListView::rowsAboutToBeInserted(const QModelIndex &parent, int start, int){
	prevSelection = parent.child(start-1, 0);
}

void TabbedListView::rowsRemoved(const QModelIndex &parent, int first, int){
	//auto prevSelection = model->mapFromSource(parent.child(first, 0));
	if(prevSelection.row() != prevSelection.row()){
		qDebug()<<"*****************\n"<<prevSelection.row() << prevSelection.row();
	}

	auto prevSelection1 = parent.child(first, 0);
	setCurrentIndex(prevSelection1);
}

void TabbedListView::rowsInserted(const QModelIndex &parent, int first, int){

	if(prevSelection.row() != prevSelection.row()){
		qDebug()<<"*****************\n"<<prevSelection.row() << prevSelection.row();
	}

	auto prevSelection1 = parent.child(first, 0);
	setCurrentIndex(prevSelection1);
}
