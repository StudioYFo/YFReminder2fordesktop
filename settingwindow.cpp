#include "settingwindow.h"
#include "ui_settingwindow.h"
#include "mainwindow.h"
SettingWindow::SettingWindow(MainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingWindow)
{
    this->mainwindow=parent;
    ui->setupUi(this);
    this->year=mainwindow->data.date.year();
    this->month=mainwindow->data.date.month();
    this->day=mainwindow->data.date.day();
    ui->spinYear->setValue(year);
    ui->spinMonth->setValue(month);
    ui->spinDay->setValue(day);
    isRunNian=((year%4==0 && year%100!=0) || year%400)?true:false;
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::on_btnBack_clicked()
{

    mainwindow->updateData();
    mainwindow->show();
    this->hide();
}

void SettingWindow::on_spinYear_valueChanged(int arg1)
{
    this->year=arg1;
    mainwindow->data.date.setDate(year,month,day);
    isRunNian=((year%4==0 && year%100!=0) || year%400)?true:false;
}

void SettingWindow::on_spinMonth_valueChanged(int arg1)
{
    this->month=arg1;
    if(arg1==2){
        if(isRunNian){
            ui->spinDay->setMaximum(29);
        }
        else{
            ui->spinDay->setMaximum(28);
        }
    }
    else{
        switch(arg1){
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        {
            ui->spinDay->setMaximum(31);
            break;
        }
        case 4:
        case 6:
        case 9:
        case 11:
        {
            ui->spinDay->setMaximum(30);
            break;
        }
        default:{
            ui->spinDay->setMaximum(31);
            break;
        }
        }
    }
    mainwindow->data.date.setDate(year,month,day);
}

void SettingWindow::on_spinDay_valueChanged(int arg1)
{
    this->day=arg1;
    mainwindow->data.date.setDate(year,month,day);
}
