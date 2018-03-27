/********************************************************************************
** Form generated from reading UI file 'OmegaEngineEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OMEGAENGINEEDITOR_H
#define UI_OMEGAENGINEEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OmegaEngineEditorClass
{
public:
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *OmegaEngineEditorClass)
    {
        if (OmegaEngineEditorClass->objectName().isEmpty())
            OmegaEngineEditorClass->setObjectName(QStringLiteral("OmegaEngineEditorClass"));
        OmegaEngineEditorClass->resize(1369, 853);
        centralWidget = new QWidget(OmegaEngineEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(20, 20, 1, 1));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 16, 16));
        scrollArea->setWidget(scrollAreaWidgetContents);
        OmegaEngineEditorClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(OmegaEngineEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        OmegaEngineEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(OmegaEngineEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        OmegaEngineEditorClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(OmegaEngineEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1369, 21));
        OmegaEngineEditorClass->setMenuBar(menuBar);

        retranslateUi(OmegaEngineEditorClass);

        QMetaObject::connectSlotsByName(OmegaEngineEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *OmegaEngineEditorClass)
    {
        OmegaEngineEditorClass->setWindowTitle(QApplication::translate("OmegaEngineEditorClass", "OmegaEngineEditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OmegaEngineEditorClass: public Ui_OmegaEngineEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OMEGAENGINEEDITOR_H
