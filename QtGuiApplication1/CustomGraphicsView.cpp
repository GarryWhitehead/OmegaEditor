#include "CustomGraphicsView.h"
#include "SpriteManager.h"
#include "OmegaEngineEditor.h"

#include <qevent.h>

CustomGraphicsView::CustomGraphicsView(MainWindow *window, QWidget *parent) : 
	QGraphicsView(parent), 
	m_window(window), 
	m_dragIndex(-1), 
	m_clipboardID(-1),
	m_moveSprite(false),
	m_collMaskEnabled(false)
{}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		if (m_collMaskEnabled) {
			m_window->SetCollisionMask(event->pos());
		}
		else {
			int newID = m_window->FindEntityID(event->pos());
			if (newID == m_dragIndex) {
				m_moveSprite = true;
			}
			else {
				m_dragIndex = newID;
				m_window->UpdateSelectedEntity(m_dragIndex);
			}
		}
	}
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->button() == Qt::LeftButton) && m_moveSprite) {
		m_window->CursorUpdateSpritePosition(m_dragIndex, event->pos());
	}
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && m_moveSprite) {
		m_window->CursorUpdateSpritePosition(m_dragIndex, event->pos());
		m_moveSprite = false;
	}
}

void CustomGraphicsView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Up|| event->key() == Qt::Key_Down || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
		m_window->KeyUpdateSpritePosition(m_dragIndex, event->key(), KEY_PRESS_MAX);
	}
	if (event->key() == Qt::Key_Delete) {
		if (m_collMaskEnabled) {
			m_window->DeleteCollisionMask();
		}
		else {
			// delete entity
			m_window->DeleteEntity(m_dragIndex);
			m_dragIndex = -1;
		}
	}
	if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {														// rotate entity
		m_window->KeyRotateSprite(m_dragIndex, event->key());
	}
	if (event->key() == Qt::Key_Escape) {
		if (m_collMaskEnabled) {
			m_collMaskEnabled = false;
			m_window->TerminateCollMask();
		}
	}

	if (event->matches(QKeySequence::Copy)) {
		if (m_dragIndex >= 0) {						// check that an entity is selected
			m_clipboardID = m_dragIndex;
		}
	}
	if (event->matches(QKeySequence::Paste)) {
		if (m_clipboardID >= 0) {
			m_window->PasteFromClipboard(m_clipboardID);
		}
		m_clipboardID = -1;
	}
	if (event->matches(QKeySequence::Cut)) {

	}
}

void CustomGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		if (m_dragIndex >= 0) {
			m_dragIndex = m_window->FindEntityID(event->pos());
			m_window->ShowEntityData(m_dragIndex);
		}
	}
}