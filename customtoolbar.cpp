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
/*
bool DriveButton::event(QEvent *e){
	if(e->type() == QEvent::FocusAboutToChange)
		return true;
	qDebug()<<	e->type();
	bool res = QPushButton::event(e);
	qDebug()<<	res;
	return res;
}*/
//bool QPushButton::event(QEvent *e)


CustomToolbar::CustomToolbar(QWidget *parent) : QToolBar(parent){
	volumes = QStorageInfo::mountedVolumes();
	refreshMountPoints();
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));//update toolbar every 1 sec
	timer->start(1000);
}

void CustomToolbar::update(){
	auto newVols = QStorageInfo::mountedVolumes();
	if(newVols.size() != volumes.size()){
		//do I need to *free* volumes first?
		volumes = newVols;
		refreshMountPoints();
	}
}

void CustomToolbar::refreshMountPoints(){
	clear();//clear mountpoint buttons from toolbar

	for(auto &drive :  volumes){
		DriveButton *button = new DriveButton(drive.rootPath(), this);
		addWidget(button);
		addSeparator();
	}

}

void CustomToolbar::sendSignal(const QString &dir) const{
	emit cdTo(dir);
}
