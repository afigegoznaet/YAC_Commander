#ifndef ORDEREDFILESYSTEMMODEL_H
#define ORDEREDFILESYSTEMMODEL_H

#include <QSortFilterProxyModel>
#include <QFileInfo>
#include <QDebug>
#include <QFileSystemModel>

class OrderedFileSystemModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit OrderedFileSystemModel(QObject *parent = Q_NULLPTR);
	bool lessThan(const QModelIndex &left,
										  const QModelIndex &right) const;
	QString rootPath(){
		return sourceModel()->rootDirectory().absolutePath();
	}

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

	void setFilter(QDir::Filters filt) const {
		sourceModel()->setFilter(filt);
	}

	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

	void sort(){
		sort(column, order);
	}
	QFileSystemModel *sourceModel() const;

private:

	int column = 0;
	Qt::SortOrder order = Qt::AscendingOrder;
};

#endif // ORDEREDFILESYSTEMMODEL_H
