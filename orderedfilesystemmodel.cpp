#include "orderedfilesystemmodel.h"
#include <QDateTime>

OrderedFileSystemModel::OrderedFileSystemModel(QObject *parent) :
	QSortFilterProxyModel(parent){

}

bool OrderedFileSystemModel::lessThan(const QModelIndex &left,
									  const QModelIndex &right) const{
	const QFileInfo &leftFile = sourceModel()->fileInfo(left);
	const QFileInfo &rightFile = sourceModel()->fileInfo(right);
	if(leftFile.isDir() ^ rightFile.isDir())
		if(this->order)
			return !leftFile.isDir();
		else
			return leftFile.isDir();		//will it be optimized
	bool res = false;
	if(!leftFile.fileName().compare(".."))
		return order==Qt::AscendingOrder ? true : false;
	if(!rightFile.fileName().compare(".."))
		return order==Qt::AscendingOrder ? false : true;
	switch(column){
	case 0:
		res = leftFile.fileName().compare(rightFile.fileName())<0;
		break;
	case 2:
		res = leftFile.suffix().compare(rightFile.suffix())<0;
		break;
	case 3:
		res = leftFile.created().toTime_t() < rightFile.created().toTime_t();
		break;
	case 1:
		res = leftFile.size() < rightFile.size();
		break;
	}

	return res;
}

void OrderedFileSystemModel::sort(int column, Qt::SortOrder order){
	this->column = column;
	this->order = order;
	QSortFilterProxyModel::sort(column, order);
}

QFileSystemModel* OrderedFileSystemModel::sourceModel() const{
	return dynamic_cast<QFileSystemModel*>(QSortFilterProxyModel::sourceModel());
}
