/********************************************************************************
** Form generated from reading UI file 'NewLevel.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLEVEL_H
#define UI_NEWLEVEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewLevelForm
{
public:
    QToolButton *toolButton;
    QLineEdit *lineEdit;
    QToolButton *toolButton_2;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpinBox *spinBox;
    QLabel *label_3;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox_3;
    QLabel *label_4;
    QLabel *label_5;
    QFrame *frame;
    QFrame *line;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QToolButton *toolButton_3;
    QLabel *label_6;
    QLabel *label_7;
    QToolButton *toolButton_4;
    QFrame *frame_2;
    QFrame *frame_3;
    QLabel *label_8;
    QFrame *frame_4;
    QLabel *label_9;

    void setupUi(QWidget *NewLevelForm)
    {
        if (NewLevelForm->objectName().isEmpty())
            NewLevelForm->setObjectName(QStringLiteral("NewLevelForm"));
        NewLevelForm->resize(584, 300);
        toolButton = new QToolButton(NewLevelForm);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(230, 60, 25, 21));
        lineEdit = new QLineEdit(NewLevelForm);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(117, 62, 113, 16));
        toolButton_2 = new QToolButton(NewLevelForm);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));
        toolButton_2->setGeometry(QRect(230, 90, 25, 21));
        lineEdit_2 = new QLineEdit(NewLevelForm);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(117, 92, 113, 16));
        label = new QLabel(NewLevelForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(32, 60, 91, 16));
        label_2 = new QLabel(NewLevelForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(47, 91, 71, 16));
        pushButton = new QPushButton(NewLevelForm);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(290, 270, 91, 23));
        pushButton_2 = new QPushButton(NewLevelForm);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(190, 270, 91, 23));
        spinBox = new QSpinBox(NewLevelForm);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(196, 176, 61, 22));
        spinBox->setMaximum(1000);
        label_3 = new QLabel(NewLevelForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(143, 178, 61, 16));
        spinBox_2 = new QSpinBox(NewLevelForm);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setGeometry(QRect(363, 158, 61, 22));
        spinBox_2->setMaximum(10000);
        spinBox_3 = new QSpinBox(NewLevelForm);
        spinBox_3->setObjectName(QStringLiteral("spinBox_3"));
        spinBox_3->setGeometry(QRect(363, 188, 61, 22));
        spinBox_3->setMaximum(10000);
        label_4 = new QLabel(NewLevelForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(302, 160, 61, 16));
        label_5 = new QLabel(NewLevelForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(300, 190, 61, 16));
        frame = new QFrame(NewLevelForm);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(123, 138, 341, 101));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        line = new QFrame(NewLevelForm);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 250, 561, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        lineEdit_3 = new QLineEdit(NewLevelForm);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(397, 92, 113, 16));
        lineEdit_4 = new QLineEdit(NewLevelForm);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(397, 62, 113, 16));
        toolButton_3 = new QToolButton(NewLevelForm);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
        toolButton_3->setGeometry(QRect(510, 60, 25, 21));
        label_6 = new QLabel(NewLevelForm);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(327, 91, 71, 16));
        label_7 = new QLabel(NewLevelForm);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(312, 60, 91, 16));
        toolButton_4 = new QToolButton(NewLevelForm);
        toolButton_4->setObjectName(QStringLiteral("toolButton_4"));
        toolButton_4->setGeometry(QRect(510, 90, 25, 21));
        frame_2 = new QFrame(NewLevelForm);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(19, 20, 261, 101));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_3 = new QFrame(frame_2);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(0, 0, 261, 101));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        label_8 = new QLabel(frame_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(86, 7, 91, 16));
        frame_4 = new QFrame(NewLevelForm);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(300, 20, 261, 101));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_9 = new QLabel(frame_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(80, 10, 121, 16));
        frame_4->raise();
        frame_2->raise();
        frame->raise();
        toolButton->raise();
        lineEdit->raise();
        toolButton_2->raise();
        lineEdit_2->raise();
        label->raise();
        label_2->raise();
        pushButton->raise();
        pushButton_2->raise();
        spinBox->raise();
        label_3->raise();
        spinBox_2->raise();
        spinBox_3->raise();
        label_4->raise();
        label_5->raise();
        line->raise();
        lineEdit_3->raise();
        lineEdit_4->raise();
        toolButton_3->raise();
        label_6->raise();
        label_7->raise();
        toolButton_4->raise();

        retranslateUi(NewLevelForm);

        QMetaObject::connectSlotsByName(NewLevelForm);
    } // setupUi

    void retranslateUi(QWidget *NewLevelForm)
    {
        NewLevelForm->setWindowTitle(QApplication::translate("NewLevelForm", "Form", nullptr));
        toolButton->setText(QApplication::translate("NewLevelForm", "...", nullptr));
        toolButton_2->setText(QApplication::translate("NewLevelForm", "...", nullptr));
        label->setText(QApplication::translate("NewLevelForm", "Texture Filename", nullptr));
        label_2->setText(QApplication::translate("NewLevelForm", "Atlas Filename", nullptr));
        pushButton->setText(QApplication::translate("NewLevelForm", "Cancel", nullptr));
        pushButton_2->setText(QApplication::translate("NewLevelForm", "Ok", nullptr));
        label_3->setText(QApplication::translate("NewLevelForm", "Level Time", nullptr));
        label_4->setText(QApplication::translate("NewLevelForm", "Level Width", nullptr));
        label_5->setText(QApplication::translate("NewLevelForm", "Level Height", nullptr));
        toolButton_3->setText(QApplication::translate("NewLevelForm", "...", nullptr));
        label_6->setText(QApplication::translate("NewLevelForm", "Atlas Filename", nullptr));
        label_7->setText(QApplication::translate("NewLevelForm", "Texture Filename", nullptr));
        toolButton_4->setText(QApplication::translate("NewLevelForm", "...", nullptr));
        label_8->setText(QApplication::translate("NewLevelForm", "Main SpriteSheet", nullptr));
        label_9->setText(QApplication::translate("NewLevelForm", "Background SpriteSheet", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewLevelForm: public Ui_NewLevelForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLEVEL_H
