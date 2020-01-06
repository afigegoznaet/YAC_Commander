#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QDialog>
#include <QFile>


class DataStorage {
public:
	virtual ~DataStorage() = default;
	virtual QByteArray getData(std::size_t position, std::size_t length) = 0;
	virtual unsigned long long size() = 0;
};


class DataStorageArray : public DataStorage {
public:
	DataStorageArray(const QByteArray &arr);
	QByteArray		   getData(std::size_t position, std::size_t length) final;
	unsigned long long size() final;

private:
	QByteArray m_data;
};

class DataStorageFile : public DataStorage {
public:
	DataStorageFile(const QString &fileName);
	QByteArray		   getData(std::size_t position, std::size_t length) final;
	unsigned long long size() final;

private:
	QFile m_file;
};


namespace Ui {
	class TextViewer;
}

class TextViewer : public QDialog {
	Q_OBJECT

public:
	explicit TextViewer(QWidget *parent = nullptr);
	~TextViewer() override;
	void setDocument(QString &&docPath);

private:
	Ui::TextViewer *ui;
	// void setFileList(QFileList&& files);
	// void insertFromMimeData( const QMimeData *source );
};


#endif // TEXTVIEWER_H
