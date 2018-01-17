#ifndef ORDEREDFILESYSTEMMODEL_H
#define ORDEREDFILESYSTEMMODEL_H

#include <QSortFilterProxyModel>
#include <QFileInfo>
#include <QDebug>
#include <QFileSystemModel>
#include <QDateTime>
#include <QMimeData>
#include "../Dialogs/FileProgressDialog.hpp"

class OrderedFileSystemModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit OrderedFileSystemModel(QObject *parent = Q_NULLPTR);
	bool lessThan(const QModelIndex &left,
						const QModelIndex &right) const override;
	QString rootPath(){
		return sourceModel()->rootDirectory().absolutePath();
	}
	/*QString parentPath(){
		return sourceModel()->rootDirectory(). .absolutePath();
	}*/
	QModelIndex setRootPath(const QString& rootPath) const {
		return mapFromSource(sourceModel()->setRootPath(rootPath));
	}
	QModelIndex getRootIndex() const {
		return mapFromSource(sourceModel()->index(sourceModel()->rootPath()));
	}
	QModelIndex getSourceRootIndex() const {
		return (sourceModel()->index(sourceModel()->rootPath()));
	}
	QFileInfo fileInfo(QModelIndex index) const {
		return sourceModel()->fileInfo(mapToSource(index));
	}

	void setFilter(QDir::Filters filt) const {sourceModel()->setFilter(filt);}
	void sort(){sort(column, order);}
	virtual void sort(int column,
					  Qt::SortOrder order = Qt::AscendingOrder) override;
	QFileSystemModel *sourceModel() const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action,
					  int row, int column, const QModelIndex &parent) override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;


signals:
	void directoryLoaded(QString);
	void setFileAction(QFileInfoList fileList, QString destination, Qt::DropAction action);
private:

	int column = 0;
	Qt::SortOrder order = Qt::AscendingOrder;
};

#endif // ORDEREDFILESYSTEMMODEL_H
