#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <alldata.h>
#include <QDate>
#include <QFont>
#include "localdatabasefile.h"
#include "weekjudge.h"
namespace Ui {
    class MainWindow;
}
class SettingWindow;
class RemoteDataBaseManager;
class QListWidgetItem;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();
    void updateData();
    AllData data;
public slots:
    void on_downloadDataBaseFileComplete();
    void on_uploadDataBaseFileComplete();
private slots:


    void on_ledReminder_returnPressed();

    void on_btnConfirm_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_MainWindow_destroyed();

    void on_btnExit_clicked();

    void on_btnSettings_clicked();

    void on_MainWindow_destroyed(QObject *arg1);

    void on_btnRemoteSync_clicked();



    void on_btnRemoteDownload_clicked();

private:
    Ui::MainWindow *ui;

    LocalDataBaseFile databasefile;
    QFont normalFont,strikeoutFont;
    SettingWindow *settingwindow;
    RemoteDataBaseManager *remotedatabasemanager;
    AllData syncdata;

    AllData combineSyncData(AllData localData,AllData SyncData);
};

#endif // MAINWINDOW_H
