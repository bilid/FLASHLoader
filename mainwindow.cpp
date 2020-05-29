#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_Port=new QSerialPort(this);//创建新的串口对象
    if(getMessageOfPorts()==false)qDebug()<<"没有得到可用串口";
    portsWindowInit();
    connect(ui->pushButton_openPort ,SIGNAL(clicked(bool)),this,SLOT(openSerialPort())) ;
    connect(ui->pushButton_closePort,SIGNAL(clicked(bool)),this,SLOT(closeSerialPort())) ;
    QObject::connect(m_Port, &QSerialPort::readyRead, this, &MainWindow::showAcceptedData );
    connect(ui->pushButtonsendText,SIGNAL(clicked(bool)),this,SLOT(sendText()));
    connect(ui->pushButton_AddFile,SIGNAL(clicked(bool)),this,SLOT(AddFile()));
    connect(ui->pushButton_SendFile,SIGNAL(clicked(bool)),this,SLOT(sendFile()));
    //     fileInf  *file=new fileInf( ) ;
    //    ui->listWidget_FileInf->addItem(file);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::getMessageOfPorts()
{
    qDebug()<<"显示串口信息";
    QString  message;//信息提示变量
    foreach (const QSerialPortInfo qspinfo, QSerialPortInfo::availablePorts())
    {
        message.append(qspinfo.portName()+' '+qspinfo.description()+' '
                       +qspinfo.manufacturer()+ ' '+qspinfo.serialNumber()+' '
                       +qspinfo.serialNumber()+'\n');
    }
    qDebug()<<message;
    // QString mgForPorts;
    if(QMessageBox::Yes ==QMessageBox::question(NULL,
                                                "串口信息",
                                                message,
                                                QMessageBox::Yes | QMessageBox::No,
                                                QMessageBox::Yes))
        return true;
    else
        return false;
}

bool MainWindow::portsWindowInit()
{
    QList<QString> portsMessages;
    foreach (const QSerialPortInfo qspinfo, QSerialPortInfo::availablePorts())
    {
        portsMessages.append(qspinfo.portName());
    }
    ui->comboBox_coms->addItems(portsMessages);
    QList<QString>  baudRD ;
    baudRD <<"9600"<<"115200" ;
    ui->comboBox_Baud->addItems(baudRD);
    return true;

}

void MainWindow::dataAnalysis(QByteArray message)
{
    qDebug()<<message;

    if(message.at(0)=='A')
    {

        QByteArray fileCent;

        if(byteArry_file.size()-(p++)*512>=512)
        {

            fileCent.append("{B");
            fileCent.append(byteArry_file.mid( (p-1) *512,512));
        }
        else
        {

            fileCent.append("{!");
            fileCent.append(byteArry_file.mid( (p-1) *512,byteArry_file.size()%512));
        }

        fileCent.append('}');
        m_Port->write(fileCent );
        qDebug()<<p;
        qDebug()<<fileCent;
    }
    else
    {
        qDebug()<<"传输完成";
        p=0;
    }
}

void MainWindow::openSerialPort()
{
    m_Port->setPortName(ui->comboBox_coms->currentText());
    m_Port->setDataTerminalReady(true);
    m_Port->setBaudRate(ui->comboBox_Baud ->currentText().toInt());
    m_Port->setDataBits(QSerialPort::Data8);
    m_Port->setParity(QSerialPort::NoParity);
    if(m_Port->open(QIODevice::ReadWrite))
    {
        ui->pushButton_closePort->setEnabled(true);
        ui->pushButton_openPort->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this,tr("警告："),tr("串口初始化失败"));
        ui->pushButton_closePort->setEnabled(false);
        ui->pushButton_openPort->setEnabled(true);
    }
}

void MainWindow::closeSerialPort()
{
    m_Port->close();
    ui->pushButton_closePort->setEnabled(false);
    ui->pushButton_openPort->setEnabled(true);
}

void MainWindow::showAcceptedData()
{
    while(m_Port->bytesAvailable()>0)
    {
        QByteArray buf =  m_Port->read(1);
        if(!buf.isEmpty())
        {
            if(flag==0)
            {
                if(buf[0]=='{')
                {
                    flag=1;
                }
            }
            else if(flag==1)
            {

                if(buf[0]=='}')
                {
                    flag=0;
                    dataAnalysis(data);
                    data.clear();
                }
                else
                {
                    data.append(buf[0]);
                }
            }
        }

    }

}

void MainWindow::sendText()
{
    QByteArray Text=ui->lineEdit_sendText->text().toUtf8();
    m_Port->write(Text);

}

void MainWindow::AddFile()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr( "选择文件" ), ".", tr("文件(*.*)"));
    //QListWidgetItem *fileItem=new QListWidgetItem();
    //fileItem->setText(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, tr("Warning"), tr("File open fail."));
        return;
    }
    byteArry_file=file.readAll();
    qDebug()<<byteArry_file.size();
}

void MainWindow::sendFile()
{

    m_Port->write("{A}" );//开始发送

}
