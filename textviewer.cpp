#include "textviewer.h"
#include "ui_textviewer.h"
#include <QDebug>
#include <QTime>
#include <QFile>
#include <QPlainTextDocumentLayout>

TextViewer::TextViewer(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TextViewer)
{
	ui->setupUi(this);
}

TextViewer::~TextViewer()
{
	delete ui;
}

void TextViewer::setDocument(QString &docPath){
	auto file = new QFile(docPath, this);
	file->open(QIODevice::ReadOnly);
	qDebug()<<"Start now: "<<QTime::currentTime();

	auto document = new QTextDocument(this);
	QString hz(QByteArray::fromRawData((char*)file->map(0,file->size()),file->size()));
	file->close();
	qDebug()<<hz.length();
	qDebug()<<"file mapped now: "<<QTime::currentTime();
	document->setPlainText(hz);
	qDebug()<<QTime::currentTime();

	auto layout = new QPlainTextDocumentLayout(document);
	document->setDocumentLayout(layout);
	ui->plainTextEdit->setDocument(document);
}
