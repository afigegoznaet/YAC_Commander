#include "filefindingsmodel.h"

FileFindingsModel::FileFindingsModel(QObject *parent) : QAbstractTableModel(parent){
	files = {"asdf","fdsa"};
}

int FileFindingsModel::rowCount(const QModelIndex &) const { return files.size();}
int FileFindingsModel::columnCount(const QModelIndex &) const {return cols;}
QVariant FileFindingsModel::data(const QModelIndex &index, int role) const{
	qDebug()<<index.row();
	qDebug()<<role;
	return files.at(index.row());
}
