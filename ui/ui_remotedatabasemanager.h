/********************************************************************************
** Form generated from reading UI file 'remotedatabasemanager.ui'
**
** Created: Sun Sep 23 04:34:41 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOTEDATABASEMANAGER_H
#define UI_REMOTEDATABASEMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "QtWebKit/QWebView"

QT_BEGIN_NAMESPACE

class Ui_RemoteDataBaseManager
{
public:
    QWidget *centralwidget;
    QWebView *webView;
    QPushButton *btnBack;

    void setupUi(QMainWindow *RemoteDataBaseManager)
    {
        if (RemoteDataBaseManager->objectName().isEmpty())
            RemoteDataBaseManager->setObjectName(QString::fromUtf8("RemoteDataBaseManager"));
        RemoteDataBaseManager->resize(360, 640);
        centralwidget = new QWidget(RemoteDataBaseManager);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        webView = new QWebView(centralwidget);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setGeometry(QRect(0, 0, 361, 571));
        webView->setUrl(QUrl("about:blank"));
        btnBack = new QPushButton(centralwidget);
        btnBack->setObjectName(QString::fromUtf8("btnBack"));
        btnBack->setGeometry(QRect(10, 580, 151, 51));
        RemoteDataBaseManager->setCentralWidget(centralwidget);

        retranslateUi(RemoteDataBaseManager);

        QMetaObject::connectSlotsByName(RemoteDataBaseManager);
    } // setupUi

    void retranslateUi(QMainWindow *RemoteDataBaseManager)
    {
        RemoteDataBaseManager->setWindowTitle(QApplication::translate("RemoteDataBaseManager", "MainWindow", 0, QApplication::UnicodeUTF8));
        btnBack->setText(QApplication::translate("RemoteDataBaseManager", "\345\220\216\351\200\200", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RemoteDataBaseManager: public Ui_RemoteDataBaseManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTEDATABASEMANAGER_H
