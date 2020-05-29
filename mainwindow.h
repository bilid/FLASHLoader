#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     QSerialPort *m_Port;//定义一个新的串口对象
     bool getMessageOfPorts();//提取可用串口
     bool portsWindowInit();//串口控件初始化
     QByteArray byteArry_file;
     int addr=0;
     int p=0;//发到第几块儿了


private:
    Ui::MainWindow *ui;
    QByteArray data;//收到的{}中的字符串
    void dataAnalysis(QByteArray message);//解析收到的{}中的字符串
    int flag=0;//是否为在{}中的标准位
private slots:
    void openSerialPort();//打开串口
    void closeSerialPort();//关闭串口
    void showAcceptedData();//显示串口接收数据在文本框
    void sendText();//发送TEXT
    void AddFile();//添加要发送的文件
    void sendFile();//发送文件

};

#endif // MAINWINDOW_H
