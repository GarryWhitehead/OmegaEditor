/********************************************************************************
** Form generated from reading UI file 'SpriteView.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPRITEVIEW_H
#define UI_SPRITEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpriteViewForm
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *SpriteViewForm)
    {
        if (SpriteViewForm->objectName().isEmpty())
            SpriteViewForm->setObjectName(QStringLiteral("SpriteViewForm"));
        SpriteViewForm->resize(718, 473);
        pushButton = new QPushButton(SpriteViewForm);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(280, 440, 111, 23));

        retranslateUi(SpriteViewForm);

        QMetaObject::connectSlotsByName(SpriteViewForm);
    } // setupUi

    void retranslateUi(QWidget *SpriteViewForm)
    {
        SpriteViewForm->setWindowTitle(QApplication::translate("SpriteViewForm", "SpriteSheet View", nullptr));
        pushButton->setText(QApplication::translate("SpriteViewForm", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpriteViewForm: public Ui_SpriteViewForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPRITEVIEW_H
