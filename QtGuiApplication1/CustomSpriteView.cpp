#include "CustomSpriteView.h"
#include "SpriteViewWin.h"
#include "utility/file_log.h"
#include <qevent.h>


CustomSpriteView::CustomSpriteView(SpriteViewWin *window, QWidget *parent) : 
	QGraphicsView(parent), 
	m_window(window)
{
}

void CustomSpriteView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {

		QPoint pos = event->pos();
		int index = m_window->FindAtlasIndex(pos);
		g_filelog->WriteLog("Index selected = " + std::to_string(index));
		m_window->OnMousePress(index);
		m_window->close();
	}
}
