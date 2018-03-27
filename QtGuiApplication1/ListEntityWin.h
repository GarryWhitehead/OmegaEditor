#pragma once

#include "ui_ListEntity.h"
#include <qwidget.h>

class NewEntityWin;

class ListEntityWin : public QWidget
{
	Q_OBJECT

public:

	ListEntityWin();

	~ListEntityWin();

private slots:

	void CloseEntListWindow();
	void RowClicked();

private:

	Ui::ListEntForm *listEntUi;
	QWidget *listEntWin;
	NewEntityWin *newEntWin;

};

