#include "filemover.h"
#include <QDebug>
#include <QStorageInfo>

/*
#ifdef _WIN32
#include <windows.h>
#endif
*/

#define MAX_READ 1048576

bool isMovable(QString &from, QString &to){
	QStorageInfo in, out;
	in.setPath(from);
	out.setPath(to);

	qDebug()<<in.rootPath();
	qDebug()<<out.rootPath();
	qDebug()<<(in == out);
	return in == out;
}

int FileMover::copy(){
	QFile sourceFile(source);
	QFile destinationFile(destination);

	sourceFile.open(QIODevice::ReadOnly);
	destinationFile.open(QIODevice::WriteOnly | QIODevice::Truncate);//Need to add a ask box for this

	quint64 totalSize = sourceFile.size();
	quint64 tempSize = 0;

	char buffer[MAX_READ];//1 Mb
	qint64 bytesRead=0;

	bytesRead = sourceFile.read(buffer,MAX_READ);

	while(bytesRead>0){
		if(destinationFile.write(buffer, bytesRead) < 0)
			return false;
		tempSize +=bytesRead;
		emit bytesProgress((uint)(tempSize*100. / totalSize*1.) );
		bytesRead = sourceFile.read(buffer,MAX_READ);
	}

	if(bytesRead < 0){
		qDebug()<<sourceFile.errorString();
		return false;
	}
	return true;
}

int FileMover::move(){

	return 10 + (int)QFile::rename(source, destination);
}
/*
void FileMover::execute(){

}
*/
FileMover::~FileMover(){

	int res = 0;
	if(!action.compare("Copy",Qt::CaseInsensitive)){
		res = this->copy();
	}else{
		if(isMovable(source, destination))
			res = this->move();
		else
			res = this->copy();
	}

	emit completed(res);
	qDebug()<<"FileMover completed?!";

	QObject::disconnect(progress);
	QObject::disconnect(status);

}



FileMover::FileMover(QString source, QString destination, QString action, QObject *parent) :
	QObject(parent), destination(destination), source(source), action(action){
	qDebug()<<"Mover constructor"<<thread();
}



