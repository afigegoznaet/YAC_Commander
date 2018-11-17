#ifndef ORDEREDFILESYSTEMMODEL_H
#define ORDEREDFILESYSTEMMODEL_H

#include <QSortFilterProxyModel>
#include <QFileInfo>
#include <QDir>

class QFileSystemModel;

class OrderedFileSystemModel : public QSortFilterProxyModel {
	Q_OBJECT
public:
	explicit OrderedFileSystemModel(QObject *parent = Q_NULLPTR);
	bool lessThan(const QModelIndex &left,
				  const QModelIndex &right) const override;
	QString rootPath();
	QModelIndex setRootPath(const QString &rootPath) const;
	QModelIndex getRootIndex() const;
	QModelIndex getSourceRootIndex() const;
	QFileInfo fileInfo(QModelIndex index) const;
	void setFilter(QDir::Filters filt) const;
	void sort() { sort(column, order); }
	void sort(int column, Qt::SortOrder order) override;
	QFileSystemModel *sourceModel() const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row,
					  int column, const QModelIndex &parent) override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;


signals:
	void directoryLoaded(QString);
	void setFileAction(QFileInfoList fileList, QString destination,
					   Qt::DropAction action);

private:
	int column = 0;
	Qt::SortOrder order = Qt::AscendingOrder;
};

#endif // ORDEREDFILESYSTEMMODEL_H
