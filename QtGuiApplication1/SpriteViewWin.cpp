#include "SpriteViewWin.h"
#include "SpriteManager.h"
#include "EntityManager.h"
#include "NewEntityWin.h"
#include <qevent.h>
#include "CustomSpriteView.h"
#include "utility/file_log.h"

SpriteViewWin::SpriteViewWin(QWidget *parent, int index)
{	
	m_spriteType = index;		
	m_parent = parent;

	spriteViewUi = new Ui::SpriteViewForm;
	spriteViewUi->setupUi(this);

	customView = new CustomSpriteView(this, this);
	this->resize(SPRITE_VIEW_WIN_WIDTH, SPRITE_VIEW_WIN_HEIGHT+ 50);
	customView->setGeometry(5, 5, SPRITE_VIEW_WIN_WIDTH - 10, SPRITE_VIEW_WIN_HEIGHT - 10);
	customView->setSceneRect(0, 0, SPRITE_VIEW_WIN_WIDTH, SPRITE_VIEW_WIN_HEIGHT);
	customView->setInteractive(true);
	customView->setMouseTracking(true);
	spriteViewUi->pushButton->setGeometry(SPRITE_VIEW_WIN_WIDTH / 2, SPRITE_VIEW_WIN_HEIGHT + 10, 111, 23);

	// initialise graphics view in the sprite view window
	scene = new QGraphicsScene();
	customView->setScene(scene);
	this->show();

	p_spriteManager->ShowSpriteSheet(scene, customView, this, index);
}

SpriteViewWin::~SpriteViewWin()
{
}

int SpriteViewWin::FindAtlasIndex(QPoint pos)
{
	return p_spriteManager->FindIndex(pos, customView, m_spriteType);
}

void SpriteViewWin::OnMousePress(int index)
{
	static_cast<NewEntityWin*>(m_parent)->UpdateSpriteComponent(index);
}

