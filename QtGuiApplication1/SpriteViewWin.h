#pragma once

#include "ui_SpriteView.h"
#include <qwidget.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>

class CustomSpriteView;

class SpriteViewWin : public QWidget
{

	Q_OBJECT

public:

	const int SPRITE_VIEW_WIN_WIDTH = 1280;
	const int SPRITE_VIEW_WIN_HEIGHT = 768;

	SpriteViewWin(QWidget *parent, int index);
	~SpriteViewWin();

	int FindAtlasIndex(QPoint pos);
	void OnMousePress(int index);

private slots:

protected:

private:

	int m_spriteType;
	QWidget *m_parent;

	QWidget *viewSpriteWin;
	QGraphicsScene *scene;
	CustomSpriteView *customView;

	Ui::SpriteViewForm *spriteViewUi;
};

