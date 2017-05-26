#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QDialog>

#include <QDebug>
#include <QTime>
#include <QFile>
#include <QPlainTextDocumentLayout>
#include <QMimeDatabase>
#include <QMimeData>
#include <HexView/QHexView.h>


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
