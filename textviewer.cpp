#include "textviewer.h"
#include "ui_textviewer.h"


TextViewer::TextViewer(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TextViewer)
{
	ui->setupUi(this);
}

TextViewer::~TextViewer(){
	delete ui;
}

void TextViewer::insertFromMimeData( const QMimeData *source ){

}

void TextViewer::setDocument(QString &&docPath){

	auto file  = new QFile(this);
	QHexView *pcntwgt = ui->hexViewer;
	if(file)
		delete file;
	file = new QFile(docPath);


	if(!file->open(QIODevice::ReadOnly))
	{
		//QMessageBox::critical(this, "File opening problem", "Problem with open file `" + fileName + "`for reading");
		return;
	}


	pcntwgt -> clear();

	QByteArray arr = QByteArray::fromRawData((char*)file->map(0,file->size()),file->size());
	file->close();

	pcntwgt -> setData(new QHexView::DataStorageArray(arr));
}
