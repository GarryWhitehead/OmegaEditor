#include "ListEntityWin.h"
#include "NewEntityWin.h"
#include "EntityManager.h"
#include <qstandarditemmodel.h>
#include <qtablewidget.h>
#include "entity.h"


ListEntityWin::ListEntityWin()
{
	listEntUi = new Ui::ListEntForm();
	listEntWin = new QWidget;

	listEntUi->setupUi(listEntWin);

	listEntWin->show();

	listEntUi->tableWidget->setRowCount(entityManager->m_entityData.size());
	listEntUi->tableWidget->setColumnCount(3);
	listEntUi->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QStringList tableHeaders = { "Entity ID", "Components", "Component Mask" };
	listEntUi->tableWidget->setHorizontalHeaderLabels(tableHeaders);

	int index = 0;
	for (auto& ent : entityManager->m_entityData) {
		listEntUi->tableWidget->insertRow(index);

		listEntUi->tableWidget->setItem(index, 0, new QTableWidgetItem(tr("%1").arg(ent.first)));
		listEntUi->tableWidget->setItem(index, 1, new QTableWidgetItem(tr("%1").arg(entityManager->m_componentData[ent.first].size())));
		listEntUi->tableWidget->setItem(index, 2, new QTableWidgetItem(tr("%1").arg(ent.second->GetCompBits())));
		++index;
	}
	
	// catch if row clikced
	connect(listEntUi->tableWidget, &QTableWidget::cellClicked, this, &ListEntityWin::RowClicked);

	// Ok clicked - create entity, components and add to map
	connect(listEntUi->pushButton, &QPushButton::clicked, this, &ListEntityWin::CloseEntListWindow);
}


ListEntityWin::~ListEntityWin()
{
}

void ListEntityWin::CloseEntListWindow()
{
	listEntWin->close();

	delete listEntWin;
	delete listEntUi;
}

void ListEntityWin::RowClicked()
{
	QWidget *w = qobject_cast<QWidget*>(sender()->parent());
	if (w) {
		int row = listEntUi->tableWidget->indexAt(w->pos()).row();
		int entID = listEntUi->tableWidget->model()->data(listEntUi->tableWidget->model()->index(row, 1)).Int;

		//newEntWin = new NewEntityWin(entID);
	}
}
