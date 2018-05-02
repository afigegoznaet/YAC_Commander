#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QDebug>
#include <QTabBar>
#include <QMenu>
#include <QMouseEvent>
#include "Views/FileTabView.hpp"

namespace Ui {
	class MainWindow;
}

class FileTabSelector : public QTabWidget
{
	Q_OBJECT
public:
    FileTabSelector(QWidget* parent = Q_NULLPTR);
    ~FileTabSelector();
	FileTableView *addNewTab(bool dup=false, QString dir="");
	void readSettings();
	void setLabel(QLabel* infoLabel){this->infoLabel = infoLabel;}
	QLabel* getLabel(){return infoLabel;}
	void init(Ui::MainWindow* ui);
	void showHidden(bool show);
	void unfocus();

public slots:
	void onDirChanged(const QString dirName, int tabIndex);
	void onFocusEvent(bool);
	void sectionResized(int logicalIndex, int oldSize, int newSize);
	void sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
	void indexChanged(int index);

signals:
	void gotResized(int logicalIndex, int oldSize, int newSize);
	void gotMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
	void focusAquired();
    void setFocusSig(FileTabSelector* tab);
	void setFileAction(QFileInfoList fileList, QString destination,
					   Qt::DropAction action);
	void deleteSelectedFiles();

protected:
	QString defaultStyle;

private:
	virtual void mousePressEvent(QMouseEvent *event) override;

private:
	QMetaObject::Connection currentHeaderResizedConnection;
	QMetaObject::Connection currentHeaderMovedConnection;
	QLabel* infoLabel;

	QAction* closeTabAction;
	QMenu *menu;

	friend class MainWindow;
};

#endif // CUSTOMTABWIDGET_H
