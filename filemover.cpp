#include "filemover.h"
#include <QDebug>
#include <windows.h>

#define MAX_READ 1048576

void FileMover::DoSomething(){
	qDebug()<<"Done something";
}

bool FileMover::copy(){
	QFile sourceFile(from);
	QFile destinationFile(destination);

	sourceFile.open(QIODevice::ReadOnly);
	destinationFile.open(QIODevice::WriteOnly | QIODevice::Truncate);//Need to add a ask box for this

	quint64 totalSize = sourceFile.size();
	quint64 tempSize = 0;

	char buffer[MAX_READ];//1 Mb
	qint64 bytesRead=0;

	bytesRead = sourceFile.read(buffer,MAX_READ);
	qDebug()<<sourceFile.errorString();
	while(bytesRead>0){
		if(destinationFile.write(buffer, bytesRead) < 0)
			return false;
		tempSize +=bytesRead;
		emit bytesProgress(100*(uint)(tempSize*1. / totalSize) );
		bytesRead = sourceFile.read(buffer,MAX_READ);
	}

	if(bytesRead < 0)
		return false;
	return true;
}

bool FileMover::move(){


	return true;
}

FileMover::~FileMover(){

	bool res = 0;
	if(!action.compare("Copy",Qt::CaseInsensitive)){
		qDebug()<<"Copy";
		res = this->copy();
	}else{
		qDebug()<<"Move";
		res = this->move();
	}

	emit completed(res);
	qDebug()<<"FileMover completed?!";

	QObject::disconnect(progress);
	QObject::disconnect(status);

}



FileMover::FileMover(QString from, QString destination, QString action, QObject *parent) :
	QObject(parent), destination(destination), from(from), action(action){
	qDebug()<<"Mover constructor"<<thread();
}



