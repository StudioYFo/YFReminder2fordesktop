/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun Sep 23 04:34:41 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *lblTagToday;
    QLabel *lblDate;
    QLabel *lblSingleWeek;
    QPushButton *btnConfirm;
    QListWidget *listWidget;
    QPushButton *btnSettings;
    QPushButton *btnExit;
    QLineEdit *ledReminder;
    QPushButton *btnRemoteSync;
    QPushButton *btnRemoteDownload;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(360, 640);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lblTagToday = new QLabel(centralWidget);
        lblTagToday->setObjectName(QString::fromUtf8("lblTagToday"));
        lblTagToday->setGeometry(QRect(10, 20, 61, 41));
        lblDate = new QLabel(centralWidget);
        lblDate->setObjectName(QString::fromUtf8("lblDate"));
        lblDate->setGeometry(QRect(90, 20, 171, 41));
        lblSingleWeek = new QLabel(centralWidget);
        lblSingleWeek->setObjectName(QString::fromUtf8("lblSingleWeek"));
        lblSingleWeek->setGeometry(QRect(270, 20, 71, 41));
        btnConfirm = new QPushButton(centralWidget);
        btnConfirm->setObjectName(QString::fromUtf8("btnConfirm"));
        btnConfirm->setGeometry(QRect(280, 70, 75, 41));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 120, 341, 441));
        btnSettings = new QPushButton(centralWidget);
        btnSettings->setObjectName(QString::fromUtf8("btnSettings"));
        btnSettings->setGeometry(QRect(10, 570, 51, 61));
        btnExit = new QPushButton(centralWidget);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        btnExit->setGeometry(QRect(300, 570, 51, 61));
        ledReminder = new QLineEdit(centralWidget);
        ledReminder->setObjectName(QString::fromUtf8("ledReminder"));
        ledReminder->setGeometry(QRect(10, 70, 261, 41));
        btnRemoteSync = new QPushButton(centralWidget);
        btnRemoteSync->setObjectName(QString::fromUtf8("btnRemoteSync"));
        btnRemoteSync->setGeometry(QRect(70, 570, 101, 61));
        btnRemoteDownload = new QPushButton(centralWidget);
        btnRemoteDownload->setObjectName(QString::fromUtf8("btnRemoteDownload"));
        btnRemoteDownload->setGeometry(QRect(190, 570, 101, 61));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        lblTagToday->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblDate->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblSingleWeek->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        btnConfirm->setText(QApplication::translate("MainWindow", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        btnSettings->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        btnExit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        btnRemoteSync->setText(QApplication::translate("MainWindow", "\344\272\221\344\270\212\344\274\240", 0, QApplication::UnicodeUTF8));
        btnRemoteDownload->setText(QApplication::translate("MainWindow", "\344\272\221\344\270\213\350\275\275", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
