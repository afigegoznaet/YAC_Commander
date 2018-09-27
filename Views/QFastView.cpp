#include "QFastView.hpp"
#include "../Dialogs/TextViewerDlg.hpp"

#include "Views/QFastView.hpp"
#include <QScrollBar>
#include <QPainter>
#include <QSize>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>

#include <QDebug>
#include <stdexcept>
#include "../Dialogs/TextViewerDlg.hpp"

QFastView::QFastView(QWidget *parent):
QAbstractScrollArea(parent),
m_pdata(nullptr)
{
	QFont theFont("DejaVu Sans Mono", 12);
	QFontMetrics metrix(theFont);
	setFont(theFont);
	setFocusPolicy(Qt::StrongFocus);
	charWidth = metrix.averageCharWidth();
}


QFastView::~QFastView()
{
	if(m_pdata)
		delete m_pdata;
}

void QFastView::setData(DataStorage *pData)
{
	verticalScrollBar()->setValue(0);
	if(m_pdata)
		delete m_pdata;
	m_pdata = pData;
	//m_cursorPos = 0;
	//resetSelection(0);
}


void QFastView::showFromOffset(unsigned long long offset)
{
	if(m_pdata && offset < m_pdata->size())
	{
		//setCursorPos(offset * 2);

		//int cursorY = m_cursorPos / (2 * BYTES_PER_LINE);

		//verticalScrollBar() -> setValue(cursorY);
	}
}

void QFastView::clear()
{
	verticalScrollBar()->setValue(0);
}

int QFastView::getNextLineStart(){
	auto charsPerLine = size().width() / charWidth;
	auto newLinePos = data.toStdString().find('\n');
	if (std::string::npos != newLinePos && newLinePos < charsPerLine)
		return newLinePos+1;
	else
		return charsPerLine;
}

int QFastView::getPrevLineStart(){
	auto charsPerLine = size().width() / charWidth;
	if(charsPerLine > firstLineIdx)
		return firstLineIdx;

	auto tempData = m_pdata->getData(firstLineIdx-charsPerLine, charsPerLine);
	auto newLinePos = tempData.toStdString().find_last_of('\n');
	if (std::string::npos != newLinePos && newLinePos < charsPerLine)
		return newLinePos+1;
	else
		return charsPerLine;
}

int QFastView::getNextPageStart(){

}

int QFastView::getPrevPageStart(){

}

void QFastView::paintEvent(QPaintEvent *event){
	if(!m_pdata)
		return;

	QPainter painter(viewport());

	QBrush def = painter.brush();
	QBrush selected = QBrush(QColor(0x6d, 0x9e, 0xff, 0xff));
	data = m_pdata->getData(firstLineIdx, 8192);
	data.replace('\0', '?');
	QString text(data);
	painter.drawText(event->rect(), text);
}
