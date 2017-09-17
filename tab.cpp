#include "tab.h"
#include "customtabwidget.h"

#define IN 1
#define OUT 0
#define cout qDebug()

TabbedListView::TabbedListView(QDir directory, QWidget *parent) :
	QTableView(parent),
	directory(directory.absolutePath()){

	infoLabel = ((CustomTabWidget*)parent)->getLabel();

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
	model->setFilter(QDir::AllEntries | QDir::NoDot | QDir::System | QDir::Hidden);

	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setModel(model);
	model->setFilterRegExp("");
	setRootIndex(model->getRootIndex());
	verticalHeader()->setVisible(false);
	//this is needed for clever file selection whn moving up and down
	connect(fModel,	SIGNAL(directoryLoaded(QString)),
			this, SLOT(setCurrentSelection(QString)));
	connect(fModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
			this, SLOT(rowsInserted(QModelIndex,int,int)));
	connect(fModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
			this,	SLOT(rowsRemoved(QModelIndex,int,int)));
	//qDebug()<<directory.absolutePath();

	setStyleSheet("QTableView::item::focus { selection-background-color: solid blue; border: 1px solid green;\
				  row-background-color: solid-green}");
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
	delete prevSelection;
	prevSelection = nullptr;
	if(in_out == IN){
		directory="..";//clever selection
		QDir parentDir(model->fileInfo(index).absoluteFilePath());
		model->setRootPath(parentDir.absolutePath());
		parentDir.cd(".");
		setRootIndex(model->getRootIndex());
	}else{
		QDir parentDir(model->rootPath());
		if(parentDir.isRoot())
			return;
		directory=parentDir.dirName();
		parentDir.cdUp();
		setRootIndex(model->setRootPath(parentDir.absolutePath()));
	}

	emit dirChanged(model->rootPath(), this->index);

}

void TabbedListView::keyPressEvent(QKeyEvent *event){

	qDebug()<<styleSheet();
	QString filter;
	QModelIndex index;
	if(currentIndex().isValid())
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
	case Qt::Key_Right:
	case Qt::Key_Left:
		break;
	case Qt::Key_Space:
		selectionModel()->setCurrentIndex(index,
			QItemSelectionModel::Select | QItemSelectionModel::Rows);
	case Qt::Key_Down:
		index = rootIndex().child(index.row()+1,0);
		if(index.isValid())
			selectionModel()->setCurrentIndex(index,
										  QItemSelectionModel::NoUpdate);
		break;
	case Qt::Key_Up:
		index = rootIndex().child(index.row()-1,0);
		if(index.isValid())
			selectionModel()->setCurrentIndex(index,
										  QItemSelectionModel::NoUpdate);
		break;
	default:
		//break;
		//qDebug()<<"key: "<<key;
		//qDebug()<<"event: "<<event;
		QAbstractItemView::keyPressEvent(event);
		break;
	}
	//qDebug()<<model->fileInfo(currentIndex()).absoluteFilePath();
}

void TabbedListView::init(){
	connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_doubleClicked(QModelIndex)));
	horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	auto hz = connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));
	qDebug()<<hz;
}

void TabbedListView::setCurrentSelection(QString){
	if(prevSelection && prevSelection->isValid()){
		selectionModel()->setCurrentIndex(*prevSelection, QItemSelectionModel::NoUpdate);
		//setCurrentIndex(*prevSelection);
		//selectionModel()->select(*prevSelection, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Columns);
		return;
	}
	int rows = model->rowCount(rootIndex());
	QModelIndex index = rootIndex().child(0,0);
	for(int i=0;i<rows;i++){
		auto ind = rootIndex().child(i,0);
		if(!directory.compare(model->fileInfo(ind).fileName())){
			index = ind;
			break;
		}
	}
	selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);
	//setCurrentIndex(index);
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
	if(!items.size())
		selectedFiles.append(
					model->fileInfo(selectionModel()->currentIndex()));
	else
		foreach (auto fileIndex, items)
			selectedFiles.append(model->fileInfo(fileIndex));
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
		this->clearSelection();
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
			qDebug()<<model->fileInfo(ind).fileName();
			selectionModel()->select(ind, selectionType );
			for(int j=1;j<columnCount;j++)
			selectionModel()->select(rootIndex().child(i,j), selectionType);
		}
	}
}

void TabbedListView::rowsRemoved(const QModelIndex &parent, int first, int){
	delete prevSelection;
	prevSelection = new QModelIndex(parent.child(first, 0));
	qDebug()<<"prevSelections set at deletion";
	qDebug()<<prevSelection->row() << " "<<prevSelection->column();
}

void TabbedListView::rowsInserted(const QModelIndex &parent, int first, int last){
	qDebug()<<"rows inserted: "<<first <<" "<<last;
	qDebug()<<parent.child(first, 0);
	delete prevSelection;
	prevSelection = nullptr;
	if(!first || !last)
		return;
	//prevSelection = new QModelIndex(parent.child(first, 0));
	qDebug()<<"prevSelections set at insertion";
	//qDebug()<<prevSelection->row() << " "<<prevSelection->column();
	//model->sort();
}

