#include "filemover.h"
#include <future>
#include <tuple>

bool fileMovement(const std::tuple<QFile&, QString&, QString&> tup){
	QFile &from = std::get<0>(tup);
	QString &destination = std::get<1>(tup);
	QString &action = std::get<2>(tup);
	if(action.compare("Copy",Qt::CaseInsensitive)){
		return from.copy(destination);
	}else{
		return from.rename(destination);
	}
}

FileMover::FileMover(QFile &from, QString &destination, QString &action, QObject *parent) : QObject(parent){

	std::tuple<QFile&, QString&, QString&> tup = std::forward_as_tuple(from, destination, action);
	std::async(std::launch::async,
								 &fileMovement,tup);
}


