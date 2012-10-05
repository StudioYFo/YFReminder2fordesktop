#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QtCore/QCoreApplication>
#include "settings.h"
#include "settingwindow.h"
#include "remotedatabasemanager.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->normalFont.setStrikeOut(false);
    this->strikeoutFont.setStrikeOut(true);

    data=databasefile.ReadFromFile();
    if(data.version!=version){
        //doing nothing,for future update
    }

    this->settingwindow=new SettingWindow(this);
    this->remotedatabasemanager=new RemoteDataBaseManager(this);

    connect(this->remotedatabasemanager,SIGNAL(downloadDataBaseFileComplete()),this,SLOT(on_downloadDataBaseFileComplete()));
    connect(this->remotedatabasemanager,SIGNAL(uploadDataBaseFileComplete()),this,SLOT(on_uploadDataBaseFileComplete()));

    this->updateData();
#if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    QFile locationfile("location.ini");
    if(locationfile.exists()){
        QTextStream stream(&locationfile);
        locationfile.open(QIODevice::ReadOnly);
        int x=0,y=0;
        stream>>x>>y;
        this->move(x,y);
        locationfile.close();
    }
#endif
}

MainWindow::~MainWindow()
{
    databasefile.SaveToFile(data.date,data.list);
    delete ui;
    data.clearlist();
#if defined(Q_OS_WIN) ||  defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    QFile locationfile("location.ini");
    QTextStream stream(&locationfile);
    locationfile.open(QIODevice::WriteOnly);
    stream<<this->x()<<" "<<this->y();
    locationfile.close();
#endif
}
void MainWindow::on_downloadDataBaseFileComplete(){
//    syncdata=databasefile.ReadFromFile();
//    data=this->combineSyncData(data,syncdata);
    data=databasefile.ReadFromFile();
    this->updateData();
    databasefile.SaveToFile(data.date,data.list);
    ui->btnRemoteDownload->setText("云下载");
    ui->btnRemoteDownload->setEnabled(true);
    ui->btnRemoteSync->setEnabled(true);
}
void MainWindow::on_uploadDataBaseFileComplete(){
    ui->btnRemoteSync->setText("云上传");
    ui->btnRemoteDownload->setEnabled(true);
    ui->btnRemoteSync->setEnabled(true);
}
void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}


void MainWindow::on_ledReminder_returnPressed()
{
    if(ui->ledReminder->text().isEmpty())return;
    this->data.list.append(new Affair(ui->ledReminder->text(),false,QDateTime::currentDateTime()));
    ui->listWidget->addItem(ui->ledReminder->text());
    ui->ledReminder->clear();
}

void MainWindow::on_btnConfirm_clicked()
{
    this->on_ledReminder_returnPressed();
}

void MainWindow::updateData()
{
    int len=data.list.length();
    ui->listWidget->clear();
    for(int i=0;i<len;i++){
        ui->listWidget->addItem(data.list.at(i)->remindstr);
        ui->listWidget->item(i)->setFont(data.list.at(i)->isCompleted?this->strikeoutFont:this->normalFont);
    }
#if defined(Q_OS_WIN) || defined(Q_OS_WIN32)
    ui->lblTagToday->setText("今天");
    ui->lblDate->setText(QDate::currentDate().toString("yyyy.M.d"));
    ui->lblSingleWeek->setText("第"+QString::number(WeekJudge::whichWeek(data.date))+"周");
#else
    ui->lblTagToday->setText(tr("今天"));
    ui->lblDate->setText(QDate::currentDate().toString("yyyy.M.d"));
    ui->lblSingleWeek->setText("第"+QString::number(WeekJudge::whichWeek(data.date))+"周");
#endif
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    bool b=!data.list.at(ui->listWidget->currentRow())->isCompleted;
    data.list.at(ui->listWidget->currentRow())->isCompleted=b;
    data.list.at(ui->listWidget->currentRow())->finalModifyDateTime=QDateTime::currentDateTime();
    ui->listWidget->currentItem()->setFont(b?this->strikeoutFont:this->normalFont);

}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int currentRow=ui->listWidget->currentRow();
    delete ui->listWidget->takeItem(currentRow);
    data.list.removeAt(currentRow);
}

void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::on_btnExit_clicked()
{
    this->close();
}

void MainWindow::on_btnSettings_clicked()
{

    this->settingwindow->show();
    this->hide();
}



void MainWindow::on_MainWindow_destroyed(QObject *arg1)
{

}

void MainWindow::on_btnRemoteSync_clicked()
{
    if(!this->remotedatabasemanager->isAuthorised()){
        this->remotedatabasemanager->show();
        this->hide();
        this->remotedatabasemanager->startAuthorise();
    }
    else{
        if(!QFile::exists("SavedReminder.txt"))return;
        ui->btnRemoteSync->setEnabled(false);
        ui->btnRemoteDownload->setEnabled(false);
        ui->btnRemoteSync->setText("正在上传");
        this->databasefile.SaveToFile(data.date,data.list);
        this->remotedatabasemanager->uploadDataBaseFile();
    }
}

AllData MainWindow::combineSyncData(AllData LocalData, AllData SyncData)
{
    AllData result;
    result.version=LocalData.version;
    result.date=LocalData.date;
    //int localLen=LocalData.list.length(),syncLen=SyncData.list.length();
    bool isFound=false;
    for(int i=0;i<LocalData.list.length();){
        OUTJ:
        for(int j=0;j<SyncData.list.length();){
            if(LocalData.list.at(i)->remindstr==SyncData.list.at(j)->remindstr){
                if(LocalData.list.at(i)->finalModifyDateTime<=SyncData.list.at(j)->finalModifyDateTime){
                    result.list.append(SyncData.list.at(j));
                }
                else{
                    result.list.append(LocalData.list.at(i));
                }
                LocalData.list.removeAt(i);
                SyncData.list.removeAt(j);
                isFound=true;
                goto OUTJ;
            }
            else{
                j++;
                isFound=false;
            }
        }
        if(!isFound){
            result.list.append(LocalData.list.at(i));
            LocalData.list.removeAt(i);
        }
    }
    while(!SyncData.list.isEmpty()){
        result.list.append(SyncData.list.takeFirst());
    }
    return result;
}
//不存在的按钮
//void MainWindow::on_btnRemoteDownload_clicked()
//{
//    if(!this->remotedatabasemanager->isAuthorised()){
//        this->remotedatabasemanager->show();
//        this->hide();
//        this->remotedatabasemanager->startAuthorise();
//    }
//    else{
//        this->remotedatabasemanager->downloadDataBaseFile();
//    }
//}

void MainWindow::on_btnRemoteDownload_clicked()
{
    if(!this->remotedatabasemanager->isAuthorised()){
        this->remotedatabasemanager->show();
        this->hide();
        this->remotedatabasemanager->startAuthorise();
    }
    else{
        if(!QFile::exists("SavedReminder.txt"))return;
        ui->btnRemoteSync->setEnabled(false);
        ui->btnRemoteDownload->setEnabled(false);
        ui->btnRemoteDownload->setText("正在下载");
        this->remotedatabasemanager->downloadDataBaseFile();
    }
}
