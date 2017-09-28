#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent) {
	defaultStyle = styleSheet();

}

void CustomTabWidget::init(){
	setTabsClosable(false);
	tabBar()->setFocusPolicy(Qt::NoFocus);
	foreach (auto& child, tabBar()->children()) {
		if(child->isWidgetType())
			((QWidget*)child)->setFocusPolicy(Qt::NoFocus);
	}
}

void CustomTabWidget::indexChanged(int index){
	//qDebug()<<objectName()<<" got a new index "<<index;
	disconnect(currentHeaderResizedConnection);
	disconnect(currentHeaderMovedConnection);;
	currentHeaderResizedConnection = connect(((TabbedListView*)widget(index))->horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this, SLOT(sectionResized(int,int,int)));
	currentHeaderMovedConnection = connect(((TabbedListView*)currentWidget())->horizontalHeader(),SIGNAL(sectionMoved(int,int,int)), this, SLOT(sectionMoved(int,int,int)));
}

void CustomTabWidget::onDirChanged(const QString dirName, int tabIndex){
	setTabText(tabIndex, dirName);
	tabBar()->setDrawBase(true);
	tabBar()->setExpanding(true);
	tabBar()->setMovable(true);
	qDebug()<<this->isActiveWindow();
	qDebug()<<this->isEnabled();
}

void CustomTabWidget::onFocusEvent(bool focused){
	if(focused){
		setStyleSheet("border: 1px solid green");
		//setStyleSheet("");
		disconnect(currentHeaderResizedConnection);
		disconnect(currentHeaderMovedConnection);
		currentHeaderResizedConnection = connect(((TabbedListView*)currentWidget())->horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this, SLOT(sectionResized(int,int,int)));
		currentHeaderMovedConnection = connect(((TabbedListView*)currentWidget())->horizontalHeader(),SIGNAL(sectionMoved(int,int,int)), this, SLOT(sectionMoved(int,int,int)));

	}
	else{
		disconnect(currentHeaderResizedConnection);
		disconnect(currentHeaderMovedConnection);
		setStyleSheet(defaultStyle);
		setStyleSheet("selection-background-color: lightblue");
		emit focusLost();
	}
}

TabbedListView* CustomTabWidget::addNewTab(bool dup, QString dir){
	TabbedListView* newTab;
	int index;
	if(dup){
		index = currentIndex();
		dir = ((TabbedListView*)widget(index))->GetDirectory();
	}

	newTab = new TabbedListView(dir,this);
	newTab->init();

	index = addTab(newTab, newTab->GetDirectory());
	newTab->setTabOrderIndex(index);
	connect(newTab,            SIGNAL(dirChanged(QString,int)),
			this,  SLOT(onDirChanged(QString,int))
			);
	connect(newTab,            SIGNAL(focusEvent(bool)),
			this,  SLOT(onFocusEvent(bool))
			);

	auto defaultState = newTab->horizontalHeader()->saveState();//header state
	QSettings settings;
	newTab->setLabel(infoLabel);
	auto headerState = settings.value("Columns", defaultState).toByteArray();
	newTab->horizontalHeader()->restoreState(headerState);
	setCurrentIndex(index);
	newTab->setFocus();
	emit currentChanged(index);
	//newTab->setCurrentIndex(newTab->currentIndex().sibling(0,0));

	return newTab;
}

void CustomTabWidget::sectionResized(int logicalIndex, int oldSize, int newSize){

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
				((TabbedListView*)widget(i))->horizontalHeader()->resizeSection(logicalIndex, newSize);

	if(currentWidget()->hasFocus())
		emit gotResized(logicalIndex, oldSize, newSize);
}

void CustomTabWidget::sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex){
	//qDebug()<< objectName()<<" Moved: "<<logicalIndex<<" "<<oldVisualIndex<<" "<<newVisualIndex;
	for(int i=0;i<count();i++)
			if(currentWidget()->hasFocus() && i==currentIndex())
				continue;
			else
				((TabbedListView*)widget(i))->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);

	if(currentWidget()->hasFocus())
		emit gotMoved(logicalIndex, oldVisualIndex, newVisualIndex);
}

void CustomTabWidget::mousePressEvent(QMouseEvent *event){
	if(event->button() != Qt::RightButton)
		return;


	QMenu *menu = new QMenu(this);
	menu->addAction("Add new tab", [=](){  addNewTab();  });
	menu->addAction("Duplicate tab",[=](){	addNewTab(true); });
	if(count()>1)
		menu->addAction("Close tab",
			[=]() {
			int index = currentIndex();
			delete widget(index);
			removeTab(index);
		});
	menu->exec(QCursor::pos());
}

CustomTabWidget::~CustomTabWidget(){

	QSettings settings;
	int count=this->count();
	settings.beginWriteArray(objectName(),count);
	for(int i=0;i<count;i++){
		settings.setArrayIndex(i);
		settings.setValue("dir", ((TabbedListView*)widget(i))->GetDirectory());
	}
	settings.endArray();
}

void CustomTabWidget::readSettings(){
	QSettings settings;
	int count = settings.beginReadArray(objectName());
	int i=0;
	do{
		settings.setArrayIndex(i++);
		addNewTab(false, settings.value("dir").toString());
	}while (--count > 0) ;
	settings.endArray();
}
