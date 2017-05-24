#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent) {
	defaultStyle = styleSheet();
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
	if(focused)
		setStyleSheet("border: 1px solid green");
	else
		setStyleSheet(defaultStyle);
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
	auto headerState = settings.value("Columns", defaultState).toByteArray();
	newTab->horizontalHeader()->restoreState(headerState);
	setCurrentIndex(index);
	newTab->setFocus();

	connect(newTab->horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this, SLOT(sectionResized(int,int,int)));

	return newTab;
}

void CustomTabWidget::sectionResized(int logicalIndex, int oldSize, int newSize){
	for(int i=0;i<count();i++)
			if(((TabbedListView*)widget(i))->horizontalHeader()->sectionSize(logicalIndex) != newSize)
			((TabbedListView*)widget(i))->horizontalHeader()->resizeSection(logicalIndex, newSize);

	if(currentWidget()->hasFocus())
		emit gotResized(logicalIndex, oldSize, newSize);
}

void CustomTabWidget::mousePressEvent(QMouseEvent *event){
	if(event->button() != Qt::RightButton)
		return;


	QMenu *menu = new QMenu();
	menu->addAction("Add new tab",
		[=](){
		addNewTab();
	});
	menu->addAction("Duplicate tab",[=](){
		addNewTab(true);
	});
	if(count())
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
