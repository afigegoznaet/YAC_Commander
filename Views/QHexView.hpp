#ifndef Q_HEX_VIEWER_H_
#define Q_HEX_VIEWER_H_

#include <QAbstractScrollArea>

class DataStorage;

class QHexView : public QAbstractScrollArea {
public:
	QHexView(QWidget *parent = nullptr);
	~QHexView() final;

public slots:
	void setData(DataStorage *pData);
	void clear();
	void showFromOffset(unsigned long long offset);

protected:
	void paintEvent(QPaintEvent *event) final;
	void keyPressEvent(QKeyEvent *event) final;
	void mouseMoveEvent(QMouseEvent *event) final;
	void mousePressEvent(QMouseEvent *event) final;

private:
	DataStorage *	  m_pdata;
	unsigned long long m_posAddr;
	unsigned long long m_posHex;
	unsigned long long m_posAscii;
	unsigned long long m_charWidth;
	unsigned long long m_charHeight;


	unsigned long long m_selectBegin{};
	unsigned long long m_selectEnd{};
	unsigned long long m_selectInit{};
	unsigned long long m_cursorPos{};


	[[nodiscard]] QSize fullSize() const;
	void				resetSelection();
	void				resetSelection(int pos);
	void				setSelection(unsigned long long pos);
	void				ensureVisible();
	void				setCursorPos(int pos);
	unsigned long long  cursorPos(const QPoint &position);
};

#endif
