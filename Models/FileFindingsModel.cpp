#include "FileFindingsModel.hpp"

FileFindingsModel::FileFindingsModel(QObject *parent) : QAbstractListModel(parent){
	files = {"asdf","fdsa"};
}

int FileFindingsModel::rowCount(const QModelIndex &) const {
	//qDebug()<<files.size();
	return files.size();
}

QVariant FileFindingsModel::data(const QModelIndex &index, int role) const{
	/*qDebug()<<index.row();
	qDebug()<<"Role: "<<role;
	qDebug()<<files.at(index.row());*/
	if(role != Qt::DisplayRole)
		return QVariant();
	return files.at(index.row());
}
/*
bool FileFindingsModel::insertRows(int row, int count, const QModelIndex &parent){
	beginInsertRows(parent, row, row+count);
	endInsertRows();
	return true;
}

bool FileFindingsModel::insertRow(int row, const QModelIndex &parent){
	return true;
}
*/
