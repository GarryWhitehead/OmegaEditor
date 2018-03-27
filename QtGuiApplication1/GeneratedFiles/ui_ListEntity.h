/********************************************************************************
** Form generated from reading UI file 'ListEntity.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTENTITY_H
#define UI_LISTENTITY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ListEntForm
{
public:
    QPushButton *pushButton;
    QTableWidget *tableWidget;

    void setupUi(QWidget *ListEntForm)
    {
        if (ListEntForm->objectName().isEmpty())
            ListEntForm->setObjectName(QStringLiteral("ListEntForm"));
        ListEntForm->resize(897, 699);
        pushButton = new QPushButton(ListEntForm);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(360, 660, 111, 31));
        tableWidget = new QTableWidget(ListEntForm);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 20, 871, 621));

        retranslateUi(ListEntForm);

        QMetaObject::connectSlotsByName(ListEntForm);
    } // setupUi

    void retranslateUi(QWidget *ListEntForm)
    {
        ListEntForm->setWindowTitle(QApplication::translate("ListEntForm", "Form", nullptr));
        pushButton->setText(QApplication::translate("ListEntForm", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListEntForm: public Ui_ListEntForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTENTITY_H
