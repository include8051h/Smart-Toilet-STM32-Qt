#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//私有槽函数
private slots:
    void on_key_refresh_COM_clicked();
    void on_key_onoff_COM_clicked();
    void on_radioButton_light_on_clicked();
    void on_radioButton_light_off_clicked();
    void on_radioButton_light_auto_clicked();
    void on_radioButton_door_open_clicked();
    void on_radioButton_door_close_clicked();
    void on_radioButton_door_auto_clicked();
    void RX_FUN();
    void on_radioButton_waterbom_on_clicked();

    void on_radioButton_waterbom_off_clicked();

    void on_radioButton_waterbom_auto_clicked();

    void on_radioButton_wind_on_clicked();

    void on_radioButton_wind_off_clicked();

    void on_radioButton_wind_auto_clicked();

    void on_pushButton_accident_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
