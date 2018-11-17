#ifndef QFASTVIEWER_HPP
#define QFASTVIEWER_HPP

#include <QAbstractScrollArea>
#include <QFuture>

class DataStorage;
class QFastView : public QAbstractScrollArea {
	Q_OBJECT
public:
	QFastView(QWidget *parent = nullptr);
	~QFastView();
signals:
	void updateScrollSize(quint32 size);

public slots:
	void setData(DataStorage *pData);
	void clear();
	void showFromOffset(unsigned long long offset);
	void setScrollSize(quint32 size);
private slots:
	void moveToLine(int lineNum);

protected:
	void paintEvent(QPaintEvent *event);
	void paintHex(QPaintEvent *event);
	void paintNormal(QPaintEvent *event);
	int getNextLineStart();
	int getPrevLineStart();
	int getNextPageStart();
	int getPrevPageStart();
	void resizeEvent(QResizeEvent *event) override;
	void keyPressEvent(QKeyEvent *event);
	// void mouseMoveEvent(QMouseEvent *event);
	// void mousePressEvent(QMouseEvent *event);
private:
	uint firstLineIdx = 0;
	int charWidth;
	int charHeight;
	int linesPerPage = 0;
	int maxChars{};
	int charsPerLine = 80;
	int linesCount = 0;

	QByteArray data;
	DataStorage *m_pdata;
	QFuture<void> futureHolder;
	QAtomicInt atomicLineNum = 0;

	void countLines();
	int getNextLinesPage();
	int getPrevLinesPerPage();
};

#endif // QFASTVIEWER_HPP
