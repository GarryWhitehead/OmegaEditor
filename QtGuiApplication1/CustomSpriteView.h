#pragma once
#include <qgraphicsview.h>

class SpriteViewWin;

class CustomSpriteView : public QGraphicsView
{
	Q_OBJECT

public:

	CustomSpriteView(SpriteViewWin *window, QWidget *parent = nullptr);
	
protected:
	void mousePressEvent(QMouseEvent *event) override;

private:

	SpriteViewWin *m_window;
};

