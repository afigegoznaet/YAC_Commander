#include "filemover.h"
#include <QDebug>

bool fileMovement(const std::tuple<QFile&, QString&, QString&> tup){
	QFile &from = std::get<0>(tup);
	QString &destination = std::get<1>(tup);
	QString &action = std::get<2>(tup);
	qDebug()<<"Starting copy";
	bool result;
	if(!action.compare("Copy",Qt::CaseInsensitive)){
		qDebug()<<"Copy";
		result = from.copy(destination);
	}else{
		qDebug()<<"Move";
		result = from.rename(destination);
	}
	qDebug()<<"Finished copying";
	return result;
}

FileMover::~FileMover(){
	QFile file(destination);
	while(result.wait_for(std::chrono::seconds(0)) != std::future_status::ready){
		if(file.exists()){
			qDebug()<<"Emiting " <<file.size();
			emit bytesMoved(file.size());
			qDebug()<<"Emited!!!!!!1";
		}
		qDebug()<<"Not there yet.";
	}

	emit completed(result.get());
}

void FileMover::start(){
	result = std::async(std::launch::async, &fileMovement,
			   std::forward_as_tuple(from, destination, action));
}

FileMover::FileMover(QFile &from, QString &destination, QString &action, QObject *parent) :
	QObject(parent), destination(destination), from(from), action(action){

}



