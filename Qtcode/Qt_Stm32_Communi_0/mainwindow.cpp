#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QSerialPort"
#include "QSerialPortInfo"

#include "QMessageBox"

QSerialPort *COM = new QSerialPort();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_available_COM->addItem(info.portName());
    }

    connect(COM, SIGNAL(readyRead()), this, SLOT(RX_FUN()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_key_refresh_COM_clicked()
{
    ui->comboBox_available_COM->clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_available_COM->addItem(info.portName());
    }


}

void MainWindow::on_key_onoff_COM_clicked()
{

    QSerialPort::BaudRate baudRate;
    QSerialPort::StopBits stopBits;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity checkBits;

    stopBits = QSerialPort::OneStop;

    dataBits = QSerialPort::Data8;

    checkBits = QSerialPort::NoParity;

    if(ui->comboBox_baudrate->currentText() == "9600")
    {
        baudRate = QSerialPort::Baud9600;
    }
    else if(ui->comboBox_baudrate->currentText() == "115200")
    {
        baudRate = QSerialPort::Baud115200;
    }



    COM->setPortName(ui->comboBox_available_COM->currentText());
    COM->setBaudRate(baudRate);
    COM->setStopBits(stopBits);
    COM->setDataBits(dataBits);
    COM->setParity(checkBits);

    if(ui->key_onoff_COM->text() == "打开串口")
    {
        ui->LED1->setStyleSheet("background-color:green");

        if(COM->open(QIODevice::ReadWrite) == true)
        {
            ui->key_onoff_COM->setText("关闭串口");
            ui->LED1->setStyleSheet("background-color:red");
        }
        else
        {
            QMessageBox::critical(this, "错误提示", "打开串口失败！");
        }
    }
    else if(ui->key_onoff_COM->text() == "关闭串口")
    {
        COM->close();
        ui->key_onoff_COM->setText("打开串口");
        ui->LED1->setStyleSheet("background-color:black");
        ui->key_light_state->setText("未知");
        ui->key_door_state->setText("未知");
        ui->key_waterbom_state->setText("未知");
        ui->key_wind_state->setText("未知");
        ui->key_lightsensor_state->setText("未知");
        ui->key_watersensor_state->setText("未知");
        ui->key_airsensor_state->setText("未知");
        ui->key_accident_state->setText("未知");
        ui->key_toilet_state->setText("未知");
    }
}

void MainWindow::on_radioButton_light_on_clicked()
{
    COM->write("Cmd_Light_On#");
    ui->key_light_state->setText("开");
}

void MainWindow::on_radioButton_light_off_clicked()
{
    COM->write("Cmd_Light_Off#");
    ui->key_light_state->setText("关");
}

void MainWindow::on_radioButton_light_auto_clicked()
{
    COM->write("Cmd_Light_Auto#");
    ui->key_light_state->setText("自动");
}

void MainWindow::on_radioButton_door_open_clicked()
{
    COM->write("Cmd_Door_Open#");
    ui->key_door_state->setText("开");
}

void MainWindow::on_radioButton_door_close_clicked()
{
    COM->write("Cmd_Door_Close#");
    ui->key_door_state->setText("关");
}

void MainWindow::on_radioButton_door_auto_clicked()
{
    COM->write("Cmd_Door_Auto#");
    ui->key_door_state->setText("自动");
}

// 添加数据缓冲确保完整性
QString buffer;

void MainWindow::RX_FUN()
{
    buffer.append(QString::fromUtf8(COM->readAll()));

    // 只处理完整的数据包
    while (buffer.contains('#'))
    {
        int endIndex = buffer.indexOf('#');
        QString packet = buffer.left(endIndex);
        buffer.remove(0, endIndex + 1);

        QStringList dataList = packet.split('#', QString::SkipEmptyParts);
        for (const QString &item : dataList) {
                // 进一步解析每个键值对
                if (item.contains(':')) {
                    QStringList keyValue = item.split(':');
                    if (keyValue.size() == 2) {
                        QString key = keyValue[0].trimmed();
                        QString value = keyValue[1].trimmed();

                        // 根据不同的键更新UI或处理数据
                        if (key == "light_state") {//灯
                            if(value == "on")
                            {
                                ui->key_light_state->setText("开");
                            }
                            else if(value == "off")
                            {
                                ui->key_light_state->setText("关");
                            }
                            else if(value == "auto")
                            {
                                ui->key_light_state->setText("自动");
                            }
                        } else if (key == "door_state") {//门
                            if(value == "inopen")
                            {
                                ui->key_door_state->setText("向内开");
                            }
                            else if(value == "outclose")
                            {
                                ui->key_door_state->setText("向外关");
                            }
                            else if(value == "outopen")
                            {
                                ui->key_door_state->setText("向外开");
                            }
                            else if(value == "inclose")
                            {
                                ui->key_door_state->setText("向里关");
                            }
                        }else if(key == "waterbom_state"){
                            if(value == "on")
                            {
                                ui->key_waterbom_state->setText("开");
                            }
                            else if(value == "off")
                            {
                                ui->key_waterbom_state->setText("关");
                            }
                            else if(value == "auto")
                            {
                                ui->key_waterbom_state->setText("自动");
                            }
                        }else if(key == "wind_state")
                        {
                            if(value == "on")
                            {
                                ui->key_wind_state->setText("开");
                            }
                            else if(value == "off")
                            {
                                ui->key_wind_state->setText("关");
                            }
                            else if(value == "auto")
                            {
                                ui->key_wind_state->setText("自动");
                            }
                        }else if(key == "lightsensor_state")
                        {
                            if(value == "light")
                            {
                                ui->key_lightsensor_state->setText("亮");
                            }
                            else if(value == "dark")
                            {
                                ui->key_lightsensor_state->setText("暗");
                            }
                            else if(value == "unknown")
                            {
                                ui->key_lightsensor_state->setText("未知");
                            }
                        }else if(key == "watersensor_state")
                        {
                            if(value == "undry")
                            {
                                ui->key_watersensor_state->setText("过高");
                            }
                            else if(value == "dry")
                            {
                                ui->key_watersensor_state->setText("正常");
                            }
                            else if(value == "unknown")
                            {
                                ui->key_watersensor_state->setText("未知");
                            }
                        }else if(key == "airsensor_state")
                        {
                            if(value == "normal")
                            {
                                ui->key_airsensor_state->setText("正常");
                            }
                            else if(value == "unnormal")
                            {
                                ui->key_airsensor_state->setText("过高");
                            }
                            else if(value == "unknown")
                            {
                                ui->key_airsensor_state->setText("未知");
                            }
                        }else if(key == "accident_state")
                        {
                            if(value == "safety")
                            {
                                ui->key_accident_state->setText("安全");
                            }
                            else if(value == "danger")
                            {
                                ui->key_accident_state->setText("危险");
                            }
                        }
                        else if(key == "toilet_state")
                        {
                            if(value == "free")
                            {
                                ui->key_toilet_state->setText("无人");
                            }
                            else if(value == "using")
                            {
                                ui->key_toilet_state->setText("有人");
                            }
                        }
                        // 在此处可继续添加其他传感器数据的处理
                    }
                }
            }
        }
}

void MainWindow::on_radioButton_waterbom_on_clicked()
{
    COM->write("Cmd_Waterbom_On#");
    ui->key_waterbom_state->setText("开");
}

void MainWindow::on_radioButton_waterbom_off_clicked()
{
    COM->write("Cmd_Waterbom_Off#");
    ui->key_waterbom_state->setText("关");
}

void MainWindow::on_radioButton_waterbom_auto_clicked()
{
    COM->write("Cmd_Waterbom_Auto#");
    ui->key_waterbom_state->setText("自动");
}

void MainWindow::on_radioButton_wind_on_clicked()
{
    COM->write("Cmd_Wind_On#");
    ui->key_wind_state->setText("开");
}

void MainWindow::on_radioButton_wind_off_clicked()
{
    COM->write("Cmd_Wind_Off#");
    ui->key_wind_state->setText("关");
}

void MainWindow::on_radioButton_wind_auto_clicked()
{
    COM->write("Cmd_Wind_Auto#");
    ui->key_wind_state->setText("自动");
}

void MainWindow::on_pushButton_accident_clicked()
{
    COM->write("Cmd_Accident#");
}
