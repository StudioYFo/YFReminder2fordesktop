/********************************************************************************
** Form generated from reading UI file 'settingwindow.ui'
**
** Created: Sun Sep 23 04:34:41 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGWINDOW_H
#define UI_SETTINGWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingWindow
{
public:
    QWidget *centralwidget;
    QLabel *lblTagBeginDate;
    QSpinBox *spinYear;
    QSpinBox *spinMonth;
    QSpinBox *spinDay;
    QPushButton *btnBack;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QMainWindow *SettingWindow)
    {
        if (SettingWindow->objectName().isEmpty())
            SettingWindow->setObjectName(QString::fromUtf8("SettingWindow"));
        SettingWindow->resize(360, 640);
        centralwidget = new QWidget(SettingWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lblTagBeginDate = new QLabel(centralwidget);
        lblTagBeginDate->setObjectName(QString::fromUtf8("lblTagBeginDate"));
        lblTagBeginDate->setGeometry(QRect(20, 20, 221, 41));
        spinYear = new QSpinBox(centralwidget);
        spinYear->setObjectName(QString::fromUtf8("spinYear"));
        spinYear->setGeometry(QRect(30, 90, 251, 131));
        spinYear->setMinimum(2012);
        spinYear->setMaximum(2050);
        spinMonth = new QSpinBox(centralwidget);
        spinMonth->setObjectName(QString::fromUtf8("spinMonth"));
        spinMonth->setGeometry(QRect(30, 240, 251, 131));
        spinMonth->setMinimum(1);
        spinMonth->setMaximum(12);
        spinDay = new QSpinBox(centralwidget);
        spinDay->setObjectName(QString::fromUtf8("spinDay"));
        spinDay->setGeometry(QRect(30, 390, 251, 131));
        spinDay->setMinimum(1);
        spinDay->setMaximum(31);
        btnBack = new QPushButton(centralwidget);
        btnBack->setObjectName(QString::fromUtf8("btnBack"));
        btnBack->setGeometry(QRect(14, 592, 161, 41));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(310, 140, 21, 41));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(310, 280, 21, 61));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(310, 430, 31, 51));
        SettingWindow->setCentralWidget(centralwidget);

        retranslateUi(SettingWindow);

        QMetaObject::connectSlotsByName(SettingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SettingWindow)
    {
        SettingWindow->setWindowTitle(QApplication::translate("SettingWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        lblTagBeginDate->setText(QApplication::translate("SettingWindow", "\345\274\200\345\255\246\347\254\254\344\270\200\345\244\251\346\227\245\346\234\237\357\274\232", 0, QApplication::UnicodeUTF8));
        btnBack->setText(QApplication::translate("SettingWindow", "\350\277\224\345\233\236", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SettingWindow", "\345\271\264", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SettingWindow", "\346\234\210", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SettingWindow", "\346\227\245", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingWindow: public Ui_SettingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWINDOW_H
