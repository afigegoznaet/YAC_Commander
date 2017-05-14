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

void CustomTabWidget::addNewTab(bool dup){
	TabbedListView* newTab;
	if(dup){
		int index = currentIndex();
		QString dir = ((TabbedListView*)widget(index))->GetDirectory();
		newTab = new TabbedListView(
					dir,this);
	}else
		newTab = new TabbedListView(this);
	newTab->init();

	int index = addTab(newTab, newTab->GetDirectory());
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
