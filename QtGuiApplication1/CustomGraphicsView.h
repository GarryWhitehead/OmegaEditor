#pragma once

#include <qobject.h>
#include <qevent.h>
#include <qgraphicsview.h>

class MainWindow;

class CustomGraphicsView : public QGraphicsView
{

	Q_OBJECT

public:

	const float KEY_PRESS_MAX = 1.0f;

	CustomGraphicsView(MainWindow *window, QWidget *parent = nullptr);
	
	int currentID() const { return m_dragIndex; }
	void SetCollisionMask(bool state) { m_collMaskEnabled = state; }

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

private:

	int32_t m_dragIndex;
	int32_t m_clipboardID;
	bool m_moveSprite;
	bool m_collMaskEnabled;

	MainWindow *m_window;
};

