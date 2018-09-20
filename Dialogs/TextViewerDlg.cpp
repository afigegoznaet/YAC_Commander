#include "TextViewerDlg.hpp"
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
/*
void TextViewer::insertFromMimeData( const QMimeData *source ){

}
*/
void TextViewer::setDocument(QString &&docPath){

	QHexView *pcntwgt = new QHexView;
	ui->tabWidget->addTab(pcntwgt, docPath);
	auto file  = new QFile(this);

	if(file)
		delete file;
	file = new QFile(docPath);

	if(!file->open(QIODevice::ReadOnly))
		return;

	pcntwgt -> clear();

	QByteArray arr = QByteArray::fromRawData((char*)file->map(0,file->size()),file->size());
	file->close();

	pcntwgt -> setData(new DataStorageArray(arr));

	pcntwgt->setFocus();
}



DataStorageArray::DataStorageArray(const QByteArray &arr)
{
	m_data = arr;
}

QByteArray DataStorageArray::getData(std::size_t position, std::size_t length)
{
	return m_data.mid(position, length);
}


unsigned long long DataStorageArray::size()
{
	return m_data.count();
}


DataStorageFile::DataStorageFile(const QString &fileName): m_file(fileName)
{
	m_file.open(QIODevice::ReadOnly);
	if(!m_file.isOpen())
		throw std::runtime_error(std::string("Failed to open file `") + fileName.toStdString() + "`");
}

QByteArray DataStorageFile::getData(std::size_t position, std::size_t length)
{
	m_file.seek(position);
	return m_file.read(length);
}


unsigned long long DataStorageFile::size()
{
	return m_file.size();
}

