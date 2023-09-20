#include "QuickAccessBar.hpp"
#include <QDebug>

#include <QTimer>
DriveButton::DriveButton(const QString &root, QWidget *parent)
	: QPushButton(root, parent), rootPath(root) {
	connect(this, SIGNAL(pressed()), this, SLOT(click()));
	setFocusPolicy(Qt::NoFocus);
}

void DriveButton::click() {
	// qDebug()<<rootPath;
	qobject_cast<QuickAccessBar *>(parent())->sendSignal(rootPath);
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
// bool QPushButton::event(QEvent *e)


QuickAccessBar::QuickAccessBar(QWidget *parent) : QToolBar(parent) {
	volumes.clear();

	refreshMountPoints();
	auto timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this,
			SLOT(update())); // update toolbar every 1 sec
	timer->start(1000);
}

void QuickAccessBar::update() {
	QList<QStorageInfo> newVols;
	for (const auto &mountPt : QStorageInfo::mountedVolumes()) {
		if (mountPt.rootPath().startsWith("/snap"))
			continue;
		if (mountPt.rootPath().startsWith("/var/lib/docker"))
			continue;
		if (mountPt.rootPath().startsWith("/run/docker"))
			continue;
		if (mountPt.rootPath().startsWith("/run/user"))
			continue;
		newVols.append(mountPt);
	}

	if (newVols.size() != volumes.size()) {
		std::swap(volumes, newVols);
		refreshMountPoints();
	}
}

void QuickAccessBar::refreshMountPoints() {
	clear(); // clear mountpoint buttons from toolbar

	for (auto &drive : volumes) {
		DriveButton *button = new DriveButton(drive.rootPath(), this);
		addWidget(button);
		addSeparator();
	}

	DriveButton *button = new DriveButton(QDir::homePath(), this);
	addWidget(button);
}

void QuickAccessBar::sendSignal(const QString &dir) const { emit cdTo(dir); }
