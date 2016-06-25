#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtGlobal>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    serialport = new QSerialPort(this);
    serialport->setBaudRate(QSerialPort::Baud57600);
    //serialport->setPortName("tnt1"); //used for linux with tty0tty
    serialport->setPortName("COM10");
    serialport->setParity(serialport->NoParity);

    serialport->setDataBits(QSerialPort::Data8);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
    serialport->open(QIODevice::ReadWrite);
    qDebug() << "Serialport initialized";
    connect(this->serialport,SIGNAL(readyRead()),this,SLOT(dataAvailable()));

    ui->setupUi(this);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataAvailable()
{
QByteArray receivedData;
QByteArray dataPrefix;
//dataPrefix[0] = 0xf0;
//dataPrefix[1] = 0x20;
//dataPrefix[2] = 0x00;
//dataPrefix[3] = 0x20;
int index;
int index2;

//QByteArray output[33];// =  {0xF0, 0x20, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E};

receivedData = serialport->readAll();


for(index = 0; index <= 2; index++)
{
serialport->blockSignals(true);
    qDebug() <<  "Serial signal raised";
    if(receivedData[index] == 0x0D)
    {
        ui->txtConsole->append("0xF0 0x02 0x0D");
        ui->txtConsole->append("Sending reply FC info...Adv");
        serialport->write(QByteArray::fromHex("F0 20 00 02 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E"));
        //serialport->write(QByteArray::fromHex("F0 20 02 03 04 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E"));
        serialport->blockSignals(false);
}
    if(receivedData[index] == 0x1F)
        {
            ui->txtConsole->append("0xDE 0x02 0x1F");
            ui->txtConsole->append("Sending reply FC info...Sensor Info");
            serialport->write(QByteArray::fromHex("F0 20 00 02 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11"));
            serialport->blockSignals(false);
    }
    if(receivedData[index] == 0x22)
        {
            ui->txtConsole->append("0xDB 0x02 0x22");
            ui->txtConsole->append("Sending reply FC info...Map Indicies");
            serialport->write(QByteArray::fromHex("F0 20"));
            serialport->blockSignals(false);
    }
    if(receivedData[index] == 0xFD)
        {
            ui->txtConsole->append("0x00 0x02 0xFD");
            ui->txtConsole->append("Sending reply FC info...Aux info");
            serialport->write(QByteArray::fromHex("F0 20 F1 F2"));
            serialport->blockSignals(false);
}

}
}
