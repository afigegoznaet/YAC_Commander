#include "FileTab.h"
#include "FileTabSelector.h"

#define IN 1
#define OUT 0

TabbedListView::TabbedListView(QDir directory, QWidget *parent) :
	QTableView(parent),
	directory(directory.absolutePath()){

    infoLabel = ((FileTabSelector*)parent)->getLabel();

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
	selectionModel()->clear();
	updateInfo();
	emit dirChanged(model->rootPath(), this->index);

}

void TabbedListView::keyPressEvent(QKeyEvent *event){
	QString filter;
	QModelIndex index;
	if(currentIndex().isValid())
		index = currentIndex();
	else
		index = rootIndex().child(0,0);
	auto key = event->key();
	QFlags<QItemSelectionModel::SelectionFlag> flags = QItemSelectionModel::NoUpdate;
	auto modifiers = ((QGuiApplication*)QGuiApplication::instance())->keyboardModifiers();
	if(modifiers & Qt::ShiftModifier)
		flags = QItemSelectionModel::Toggle | QItemSelectionModel::Rows;
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
			QItemSelectionModel::Toggle | QItemSelectionModel::Rows);
	case Qt::Key_Down:
		selectionModel()->setCurrentIndex(index,
									  flags);
		index = rootIndex().child(index.row()+1,0);
		if(index.isValid())
			selectionModel()->setCurrentIndex(index,
										  QItemSelectionModel::NoUpdate);
		break;
	case Qt::Key_Up:
		selectionModel()->setCurrentIndex(index,
									  flags);
		index = rootIndex().child(index.row()-1,0);
		if(index.isValid())
			selectionModel()->setCurrentIndex(index,
										  QItemSelectionModel::NoUpdate);
		break;
	default:
		QAbstractItemView::keyPressEvent(event);
		break;
	}
	//qDebug()<<model->fileInfo(currentIndex()).absoluteFilePath();
}

void TabbedListView::init(){

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

    setSelectionMode(QAbstractItemView::NoSelection);
    setModel(model);
    model->setFilterRegExp("");
    setRootIndex(model->getRootIndex());
    verticalHeader()->setVisible(false);

	connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_doubleClicked(QModelIndex)));
	horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));
	selectionModel()->select(QModelIndex(rootIndex().child(1,0)),  QItemSelectionModel::Current);


    //this is needed for clever file selection whn moving up and down
    connect(fModel,	&QFileSystemModel::directoryLoaded,
            [&](){model->sort();});
    connect(model,	SIGNAL(directoryLoaded(QString)),
            this, SLOT(setCurrentSelection(QString)));
    connect(fModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,	SLOT(rowsRemoved(QModelIndex,int,int)));

    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &TabbedListView::updateInfo);

	delegate = new TableItemDelegate(this);
    setItemDelegate(delegate);
    connect(horizontalHeader(), &QHeaderView::geometriesChanged,
            [&](){itemDelegate()->setRect(horizontalHeader()->geometry());});
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            delegate, SLOT(currentChanged(QModelIndex,QModelIndex)));
    connect(selectionModel(), &QItemSelectionModel::currentChanged,
            [&](QModelIndex current, QModelIndex prev){
                for(int i=0;i<4;i++)
                    update(current.sibling(current.row(),i));
                for(int i=0;i<4;i++)
                    update(prev.sibling(prev.row(),i));
            });
	/*
    setStyleSheet("\
        QTableView {\
            show-decoration-selected: 1;\
        }\
        \
        QTableView::item {\
            border: 1px solid #d9d9d9;\
            border-top-color: transparent;\
            border-bottom-color: transparent;\
        }\
        \
        QTableView::item:hover {\
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\
            border: 1px solid #bfcde4;\
        }\
        QTableView::item:focus {\
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\
            border: 1px solid #bfcde4;\
        }\
        \
        QTableView::item:selected {\
            border: 1px solid #567dbc;\
        }\
        \
        QTableView::item:selected:active{\
            background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc);\
        }\
        \
        QTableView::item:selected:!active {\
            background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #6f9be8, stop: 1 #517fbf);\
        }");
*/
}

void TabbedListView::setCurrentSelection(QString){
	if(prevSelection && prevSelection->isValid()){
		qDebug()<<"Prev selection set: "<<prevSelection->row();
		setCurrentIndex(*prevSelection);
		qDebug()<<currentIndex();
		return;
	}
	int rows = model->rowCount(rootIndex());
	auto rootIndex = model->getRootIndex();
	QModelIndex index = rootIndex.child(0,0);
	int i;
	for(i=0;i<rows;i++){
		auto ind = rootIndex.child(i,0);
		//qDebug()<<"Dir: "<<directory<<" _ "<<model->fileInfo(ind).fileName();
		if(!directory.compare(model->fileInfo(ind).fileName())){
			index = ind;
			break;
		}
	}

	selectionModel()->setCurrentIndex(index,QItemSelectionModel::NoUpdate );
}

void TabbedListView::headerClicked(int section){

	Qt::SortOrder order = Qt::AscendingOrder;
	if(section == horizontalHeader()->sortIndicatorSection())
		if(Qt::AscendingOrder == horizontalHeader()->sortIndicatorOrder())
			order = Qt::DescendingOrder;
	model->sort(section, order);
}

void TabbedListView::focusInEvent(QFocusEvent *event){
	QWidget::focusInEvent(event);
	emit focusEvent(true);
	updateInfo();
}

void TabbedListView::focusOutEvent(QFocusEvent *event){
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
	model->setRootPath(dir);
	setRootIndex(model->getRootIndex());
	emit dirChanged(dir, this->index);
	updateInfo();
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
			//qDebug()<<model->fileInfo(ind).fileName();
			selectionModel()->select(ind, selectionType );
			for(int j=1;j<columnCount;j++)
			selectionModel()->select(rootIndex().child(i,j), selectionType);
		}
	}
}

void TabbedListView::rowsRemoved(const QModelIndex &parent, int first, int){
	delete prevSelection;
	prevSelection = new QModelIndex(currentIndex().sibling(first, 0));

	qDebug()<<"First: "<<first;
	setCurrentIndex(*prevSelection);
	qDebug()<<"Via new: "<<currentIndex();
	//setCurrentIndex(parent.child(first, 0));
	delegate->currentChanged(*prevSelection, *prevSelection);
	qDebug()<<"Via sibling: "<<currentIndex();
}

void TabbedListView::rowsInserted(const QModelIndex &parent, int first, int last){
	//delete prevSelection;
	//prevSelection = new QModelIndex(parent.child(last, 0));
	//setCurrentIndex(parent.child(last, 0));
}

void TabbedListView::updateInfo(){
	//qDebug()<<"***************************************************";
	//qDebug()<<"Root path:"<< model->rootPath();
	QStorageInfo storage(model->rootPath());
	//qDebug()<<"Root path:"<< storage.rootPath();
	QString fmt;
	double sizeTotal, sizeRemaining;
	QString typeTotal, typeRemaining;
	if(storage.bytesTotal() > 1024*1024*1024){
		sizeTotal = storage.bytesTotal() /1024.0/1024/1024;
		typeTotal = "GB";
	}else{
		sizeTotal = storage.bytesTotal() /1024.0/1024;
		typeTotal = "MB";
	}

	if(storage.bytesAvailable() > 1024*1024*1024){
		sizeRemaining = storage.bytesAvailable() /1024.0/1024/1024;
		typeRemaining = "GB";
	}else{
		sizeRemaining = storage.bytesAvailable() /1024.0/1024;
		typeRemaining = "MB";
	}
	fmt +=QString::number(sizeRemaining)+" "+typeRemaining  +" available of "+QString::number(sizeTotal)+  " "+typeTotal;
	fmt += "\t" + QString::number(selectionModel()->selectedRows().size()) + " selected of "+QString::number(model->rowCount(rootIndex())) +" directory items";
	//qDebug() << "size:" << sizeTotal << "MB";
	//qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
	//qDebug() << "Selected: "<<selectionModel()->selectedRows().size()<<" of "<<model->rowCount();

	infoLabel->setText(fmt);
}

void TabbedListView::goToFile(QString& fullFilePath){
	//qDebug()<<fullFilePath;
	QFileInfo info(fullFilePath);
	//qDebug()<<info.absolutePath();
	cdTo(info.absolutePath());
	directory = info.fileName();
	delete prevSelection;
	prevSelection = nullptr;
	setCurrentSelection("");
}
