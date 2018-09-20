#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QDialog>

#include <QDebug>
#include <QTime>
#include <QFile>
#include <QPlainTextDocumentLayout>
#include <QMimeDatabase>
#include <QMimeData>
#include <Views/QHexView.hpp>

class DataStorage
{
	public:
		virtual ~DataStorage() {}
		virtual QByteArray getData(std::size_t position, std::size_t length) = 0;
		virtual unsigned long long size() = 0;
};


class DataStorageArray: public DataStorage
{
	public:
		DataStorageArray(const QByteArray &arr);
		virtual QByteArray getData(std::size_t position, std::size_t length);
		virtual unsigned long long  size();
	private:
		QByteArray    m_data;
};

class DataStorageFile: public DataStorage
{
	public:
		DataStorageFile(const QString &fileName);
		virtual QByteArray getData(std::size_t position, std::size_t length);
		virtual unsigned long long  size();
	private:
		QFile      m_file;
};


namespace Ui {
class TextViewer;
}

class TextViewer : public QDialog
{
	Q_OBJECT

public:
	explicit TextViewer(QWidget *parent = 0);
	~TextViewer();
	void setDocument(QString &&docPath);

private:
	Ui::TextViewer *ui;
	//void setFileList(QFileList&& files);
	//void insertFromMimeData( const QMimeData *source );
};



#endif // TEXTVIEWER_H
