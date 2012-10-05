#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QMainWindow>

namespace Ui {
    class SettingWindow;
}
class MainWindow;
class SettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingWindow(MainWindow *parent = 0);
    ~SettingWindow();

private slots:
    void on_btnBack_clicked();

    void on_spinYear_valueChanged(int arg1);

    void on_spinMonth_valueChanged(int arg1);

    void on_spinDay_valueChanged(int arg1);

private:
    Ui::SettingWindow *ui;
    MainWindow *mainwindow;
    int year,month,day;
    bool isRunNian;
};

#endif // SETTINGWINDOW_H
