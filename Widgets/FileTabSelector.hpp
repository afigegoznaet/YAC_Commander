#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QFileInfoList>

class FileTableView;
class QLabel;
class QMenu;


namespace Ui {
	class MainWindow;
}

class FileTabSelector : public QTabWidget {
	Q_OBJECT
public:
	FileTabSelector(QWidget *parent = Q_NULLPTR);
	~FileTabSelector() final;
	FileTableView *addNewTab(bool dup = false, QString dir = "");
	void		   readSettings();
	void		   setLabel(QLabel *infoLabel) { this->infoLabel = infoLabel; }
	QLabel *	   getLabel() { return infoLabel; }
	void		   init(Ui::MainWindow *ui);
	void		   showHidden(bool show);
	void		   unfocus();

public slots:
	void onDirChanged(const QString &dirName, int tabIndex);
	void onFocusEvent(bool);
	void sectionResized(int logicalIndex, int oldSize, int newSize);
	void sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
	void indexChanged(int index);

signals:
	void gotResized(int logicalIndex, int oldSize, int newSize);
	void gotMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
	void focusAquired();
	void setFocusSig(FileTabSelector *tab);
	void setFileAction(QFileInfoList fileList, QString destination,
					   Qt::DropAction action);
	void deleteSelectedFiles();

private:
	QString defaultStyle;

private:
	void mousePressEvent(QMouseEvent *event) override;

private:
	QMetaObject::Connection currentHeaderResizedConnection;
	QMetaObject::Connection currentHeaderMovedConnection;
	QAction *				closeTabAction{};
	QLabel *				infoLabel{};
	QMenu *					menu;

	QPalette defaultPalette;
	QPalette highlightedPalette;

	friend class MainWindow;
};

#endif // CUSTOMTABWIDGET_H
