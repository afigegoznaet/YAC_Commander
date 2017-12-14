#ifndef ITEMCONTEXTMENU_HPP
#define ITEMCONTEXTMENU_HPP

#include <QMenu>

class ItemContextMenu : public QMenu
{
	Q_OBJECT
public:
	explicit ItemContextMenu(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // ITEMCONTEXTMENU_HPP
