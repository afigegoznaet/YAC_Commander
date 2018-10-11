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
	charHeight = metrix.height();
}


QFastView::~QFastView()
{
	if(m_pdata)
		delete m_pdata;
}

void QFastView::resizeEvent(QResizeEvent *event){
	//charsPerLine = size().width() / charWidth;
	linesPerPage = size().height() / charHeight;
	maxChars = charsPerLine * linesPerPage;
	QAbstractScrollArea::resizeEvent(event);
}

void QFastView::setData(DataStorage *pData)
{
	verticalScrollBar()->setValue(0);
	if(m_pdata)
		delete m_pdata;
	m_pdata = pData;
	//verticalScrollBar()->setMaximum()
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
	int linesPassed = 0;
	int pos = 0;
	while(linesPassed < linesPerPage){
		auto newLinePos = data.toStdString().find('\n',pos);
		if( newLinePos < 0 )
			return pos + (maxChars - charsPerLine*linesPassed);
		linesPassed += newLinePos/charsPerLine + 1;
		pos += (newLinePos+1);
	}
	return pos;
}

int QFastView::getPrevPageStart(){
	int linesRemaining = linesPerPage;
	int pos = maxChars;
	auto data = m_pdata->getData(firstLineIdx-maxChars, maxChars);
	while(linesRemaining > 0 ){
		auto newLinePos = data.toStdString().find_last_of('\n',pos-1);
		if( newLinePos < 0 )
			return pos - (maxChars - charsPerLine*linesRemaining);
		linesRemaining += (pos - newLinePos)/charsPerLine + 1;
		pos -= newLinePos;
	}
	return maxChars-pos;
}

void QFastView::paintEvent(QPaintEvent *event){
	if(!m_pdata)
		return;

	QPainter painter(viewport());
	QRect rect (event->rect().left(), event->rect().top(), charsPerLine * charWidth, event->rect().height());


	QBrush def = painter.brush();
	QBrush selected = QBrush(QColor(0x6d, 0x9e, 0xff, 0xff));
	data = m_pdata->getData(firstLineIdx, 8192);
	data.replace('\0', '?');
	QString text(data);
	painter.drawText(rect, text);
}


void QFastView::keyPressEvent(QKeyEvent *event)
{
	bool setVisible = false;

/*****************************************************************************/
/* Cursor movements */
/*****************************************************************************/
	/*if(event->matches(QKeySequence::MoveToNextChar))
	{
		setCursorPos(m_cursorPos + 1);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToPreviousChar))
	{
		setCursorPos(m_cursorPos - 1);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToEndOfLine))
	{
		setCursorPos(m_cursorPos | ((BYTES_PER_LINE * 2) - 1));
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToStartOfLine))
	{
		setCursorPos(m_cursorPos | (m_cursorPos % (BYTES_PER_LINE * 2)));
		resetSelection(m_cursorPos);
		setVisible = true;
	}*/
	if(event->matches(QKeySequence::MoveToPreviousLine))
	{
		//setCursorPos(m_cursorPos - BYTES_PER_LINE * 2);
		//resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToNextLine))
	{
		//setCursorPos(m_cursorPos + BYTES_PER_LINE * 2);
		//resetSelection(m_cursorPos);
		setVisible = true;
	}

	if(event->matches(QKeySequence::MoveToNextPage))
	{
		//setCursorPos(m_cursorPos + (viewport()->height() / m_charHeight - 1) * 2 * BYTES_PER_LINE);
		//resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToPreviousPage))
	{
		//setCursorPos(m_cursorPos - (viewport()->height() / m_charHeight - 1) * 2 * BYTES_PER_LINE);
		//resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToEndOfDocument))
	{
		//if(m_pdata)
			//setCursorPos(m_pdata->size() * 2);
		//resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToStartOfDocument))
	{
		//setCursorPos(0);
		//resetSelection(m_cursorPos);
		setVisible = true;
	}

/*****************************************************************************/
/* Select commands */
/*****************************************************************************/
/*
	if (event->matches(QKeySequence::SelectAll))
	{
		resetSelection(0);
		if(m_pdata)
			setSelection(2 * m_pdata->size() + 1);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectNextChar))
	{
		int pos = m_cursorPos + 1;
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectPreviousChar))
	{
		int pos = m_cursorPos - 1;
		setSelection(pos);
		setCursorPos(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectEndOfLine))
	{
		int pos = m_cursorPos - (m_cursorPos % (2 * BYTES_PER_LINE)) + (2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectStartOfLine))
	{
		int pos = m_cursorPos - (m_cursorPos % (2 * BYTES_PER_LINE));
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectPreviousLine))
	{
		int pos = m_cursorPos - (2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectNextLine))
	{
		int pos = m_cursorPos + (2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}

	if (event->matches(QKeySequence::SelectNextPage))
	{
		int pos = m_cursorPos + (((viewport()->height() / m_charHeight) - 1) * 2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectPreviousPage))
	{
		int pos = m_cursorPos - (((viewport()->height() / m_charHeight) - 1) * 2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectEndOfDocument))
	{
		int pos = 0;
		if(m_pdata)
			pos = m_pdata->size() * 2;
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectStartOfDocument))
	{
		int pos = 0;
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}

	if (event->matches(QKeySequence::Copy))
	{
		if(m_pdata)
		{
			QString res;
			int idx = 0;
			int copyOffset = 0;

			QByteArray data = m_pdata->getData(m_selectBegin / 2, (m_selectEnd - m_selectBegin) / 2 + 1);
			if(m_selectBegin % 2)
			{
				res += QString::number((data.at((idx+1) / 2) & 0xF), 16);
				res += " ";
				idx++;
				copyOffset = 1;
			}

			int selectedSize = m_selectEnd - m_selectBegin;
			for (;idx < selectedSize; idx+= 2)
			{
				QString val = QString::number((data.at((copyOffset + idx) / 2) & 0xF0) >> 4, 16);
				if(idx + 1 < selectedSize)
				{
					val += QString::number((data.at((copyOffset + idx) / 2) & 0xF), 16);
					val += " ";
				}
				res += val;

				if((idx/2) % BYTES_PER_LINE == (BYTES_PER_LINE - 1))
					res += "\n";
			}
			QClipboard *clipboard = QApplication::clipboard();
			clipboard -> setText(res);
		}
	}

	if(setVisible)
		ensureVisible();
		*/
	viewport() -> update();
}
/*
void QFastView::mouseMoveEvent(QMouseEvent * event)
{
	int actPos = cursorPos(event->pos());
	setCursorPos(actPos);
	setSelection(actPos);

	viewport() -> update();
}

void QFastView::mousePressEvent(QMouseEvent * event)
{
	int cPos = cursorPos(event->pos());

	if((QApplication::keyboardModifiers() & Qt::ShiftModifier) && event -> button() == Qt::LeftButton)
		setSelection(cPos);
	else
		resetSelection(cPos);

	setCursorPos(cPos);

	viewport() -> update();
}
*/
