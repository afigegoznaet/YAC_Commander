#include "customtoolbar.h"

DriveButton::DriveButton(QString root, QWidget *parent) :
	QPushButton(root, parent), rootPath(root){
	connect(this, SIGNAL(pressed()), this, SLOT(click())
				);
	setFocusPolicy(Qt::NoFocus);
}

void DriveButton::click(){
	qDebug()<<rootPath;
	((CustomToolbar*)parent())->sendSignal(rootPath);
}

bool DriveButton::event(QEvent *e){
	if(e->type() == QEvent::FocusAboutToChange)
		return true;
	qDebug()<<	e->type();
	bool res = QPushButton::event(e);
	qDebug()<<	res;
	return res;
}
//bool QPushButton::event(QEvent *e)


CustomToolbar::CustomToolbar(QWidget *parent) : QToolBar(parent){
  refreshMountPoints();
}

void CustomToolbar::refreshMountPoints(){
	clear();
	auto volumes = QStorageInfo::mountedVolumes();
	for(auto &drive :  volumes){
		DriveButton *button = new DriveButton(drive.rootPath(), this);
		//QPushButton *button=new QPushButton(drive.rootPath(), this);
		//QAction* mountPoint = new QAction(button);
		addWidget(button);
		addSeparator();
	}

}

void CustomToolbar::sendSignal(const QString &dir) const{
	emit cdTo(dir);
}
