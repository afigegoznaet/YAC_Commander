#include "FileTabSelector.hpp"
#include "mainwindow.hpp"

FileTabSelector::FileTabSelector(QWidget *parent) : QTabWidget(parent) {
	defaultStyle = styleSheet();
	connect(this->tabBar(), &QTabBar::tabBarClicked,[&](){emit setFocusSig(this);});

	menu = new QMenu(this);


}

QString showableName(const QString dirName){
	QString newName = dirName;
	if(dirName.length()>50){
		QStringList dirs = dirName.split('/', QString::SkipEmptyParts);
		if(dirs.size() < 3)
			return newName;
		if(!dirs.first().compare("/"))
			dirs.pop_front();
		newName = "/"+dirs.first()+"/../"+ dirs.last();
	}
	return newName;
}

void FileTabSelector::init(Ui::MainWindow *ui){
	setTabsClosable(false);
	tabBar()->setFocusPolicy(Qt::NoFocus);
	foreach (auto& child, tabBar()->children()) {
		if(child->isWidgetType())
			((QWidget*)child)->setFocusPolicy(Qt::NoFocus);
	}
	closeTabAction = ui->actionClose_tab;
	menu->addAction(ui->actionAdd_tab);
	menu->addAction(ui->actionCopy_tab);
	menu->addAction(ui->actionClose_tab);

}

void FileTabSelector::indexChanged(int index){
	//qDebug()<<objectName()<<" got a new index "<<index;
	disconnect(currentHeaderResizedConnection);
	disconnect(currentHeaderMovedConnection);;
	currentHeaderResizedConnection =
			connect(((FileTableView*)widget(index))->horizontalHeader(),
			SIGNAL(sectionResized(int,int,int)), this,
					SLOT(sectionResized(int,int,int)));
	currentHeaderMovedConnection =
			connect(((FileTableView*)currentWidget())->horizontalHeader(),
			SIGNAL(sectionMoved(int,int,int)), this,
					SLOT(sectionMoved(int,int,int)));
}

void FileTabSelector::onDirChanged(const QString dirName, int tabIndex){

	setTabText(tabIndex, showableName(dirName));
	setTabToolTip(tabIndex, dirName);
	tabBar()->setDrawBase(true);
	tabBar()->setExpanding(true);
	tabBar()->setMovable(true);
	//qDebug()<<"Is active window?: "<<this->isActiveWindow();
	//qDebug()<<this->isEnabled();
}

void FileTabSelector::onFocusEvent(bool focused){
	if(focused){
		setStyleSheet("QTabWidget::pane, QTabWidget::tab-bar {border: 2px solid green}");
		infoLabel->setStyleSheet("QLabel { background-color : navy; color : white; }");
		disconnect(currentHeaderResizedConnection);
		disconnect(currentHeaderMovedConnection);
		currentHeaderResizedConnection =
				connect(((FileTableView*)currentWidget())->horizontalHeader(),
						SIGNAL(sectionResized(int,int,int)), this,
						SLOT(sectionResized(int,int,int)));
		currentHeaderMovedConnection =
				connect(((FileTableView*)currentWidget())->horizontalHeader(),
						SIGNAL(sectionMoved(int,int,int)), this,
						SLOT(sectionMoved(int,int,int)));
		emit focusAquired();
	}
}

void FileTabSelector::unfocus(){
	infoLabel->setStyleSheet("QLabel { background-color : gray; color : white; }");
	disconnect(currentHeaderResizedConnection);
	disconnect(currentHeaderMovedConnection);
	setStyleSheet(defaultStyle);
	setStyleSheet("selection-background-color: lightblue");
}

FileTableView *FileTabSelector::addNewTab(bool dup, QString dir){
	FileTableView* newTab;
	int index;
	if(dup){
		index = currentIndex();
		dir = ((FileTableView*)widget(index))->GetDirectory();
	}

	newTab = new FileTableView(dir,this);
	newTab->init();

	index = addTab(newTab, newTab->GetDirectory());
	setTabText(index, showableName(newTab->GetDirectory()));
	setTabToolTip(index, newTab->GetDirectory());
	newTab->setTabOrderIndex(index);
	connect(newTab, SIGNAL(dirChanged(QString,int)),
			this,  SLOT(onDirChanged(QString,int)));
	connect(newTab, SIGNAL(focusEvent(bool)), this,  SLOT(onFocusEvent(bool)));
	connect(newTab, SIGNAL(setFileAction(QFileInfoList,QString,Qt::DropAction)),
			this, SIGNAL(setFileAction(QFileInfoList,QString,Qt::DropAction)));
	newTab->setLabel(infoLabel);
	auto defaultState = newTab->horizontalHeader()->saveState();//header state

	QSettings settings;
	QString settingsSection("LeftColumns");
	if(this->objectName().startsWith("right"))
		settingsSection = ("RightColumns");

	auto headerState = settings.value(settingsSection, defaultState).toByteArray();
	auto header = newTab->horizontalHeader();
	header->restoreState(headerState);


	setCurrentIndex(index);

	newTab->headerClicked(header->sortIndicatorSection());
	newTab->headerClicked(header->sortIndicatorSection());

	newTab->setFocus();


	emit currentChanged(index);
	//newTab->setCurrentIndex(newTab->currentIndex().sibling(0,0));

	return newTab;
}

void FileTabSelector::sectionResized(int logicalIndex, int oldSize, int newSize){

	//qDebug()<< objectName()<<" Resized: "<<logicalIndex<<" "<<oldSize<<" "<<newSize;

	static int check = 0;

	if(check == newSize)
		return;
	else
		check = newSize;

	for(int i=0;i<count();i++)
			if(widget(i)->hasFocus() )
				continue;
			else
				((FileTableView*)widget(i))->horizontalHeader()->resizeSection(
						logicalIndex, newSize);

	if(currentWidget()->hasFocus())
		emit gotResized(logicalIndex, oldSize, newSize);
}

void FileTabSelector::sectionMoved(int logicalIndex, int oldVisualIndex,
								   int newVisualIndex){
	for(int i=0;i<count();i++)
			if(currentWidget()->hasFocus() && i==currentIndex())
				continue;
			else
				((FileTableView*)widget(i))->horizontalHeader()->moveSection(
						oldVisualIndex, newVisualIndex);

	if(currentWidget()->hasFocus())
		emit gotMoved(logicalIndex, oldVisualIndex, newVisualIndex);
}

void FileTabSelector::mousePressEvent(QMouseEvent *event){

	if(event->button() != Qt::RightButton)
		return setFocusSig(this);

	setCurrentIndex(tabBar()->tabAt(event->pos()));




	if(count()<=1)
		closeTabAction->setDisabled(true);
	else
		closeTabAction->setEnabled(true);
	menu->exec(QCursor::pos());
	setFocusSig(this);
}

FileTabSelector::~FileTabSelector(){

	QSettings settings;
	int count=this->count();
	settings.beginWriteArray(objectName(),count);
	for(int i=0;i<count;i++){
		settings.setArrayIndex(i);
		settings.setValue("dir", ((FileTableView*)widget(i))->GetDirectory());
	}
	settings.endArray();
}

void FileTabSelector::readSettings(){
	QSettings settings;
	int count = settings.beginReadArray(objectName());
	int i=0;
	do{
		settings.setArrayIndex(i++);
		addNewTab(false, settings.value("dir").toString());
	}while (--count > 0) ;
	settings.endArray();
}

void FileTabSelector::showHidden(bool show){
	int count=this->count();
	for(int i=0;i<count;i++)
		((FileTableView*)widget(i))->showHidden(show);
}

