//  file mainwindow.cpp
//  Power FC Simulator simulates the complete Power FC MAP of a RX7 FD3S Version 4.11
//  All Sensor values are static Values
//  Copyright Bastian Gschrey and Markus Ippy
//  author Bastian Gschrey and Markus Ippy


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtGlobal>
#include <QTimer>
#include <QThread>
#include <QtSerialPort/QSerialPortInfo>

QByteArray Advanced = (QByteArray::fromHex("f02000001d2836091d02000000000000ff3602015d5c007300000000004c000025"));
QByteArray Basic = (QByteArray::fromHex("da1600000000000000000000f60200005c005b007d00e3"));




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{



    ui->setupUi(this);
    fillPortsParameters();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillPortsParameters()
{

foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
    ui->comboBox->addItem(info.portName());
    }

}




void MainWindow::dataAvailable()
{

    QByteArray receivedData;
    QByteArray Lignition1;
    QByteArray Lignition2;
    QByteArray Lignition3;
    QByteArray Lignition4;
    QByteArray Tignition1;
    QByteArray Tignition2;
    QByteArray Tignition3;
    QByteArray Tignition4;
    QByteArray injcorrection1;
    QByteArray injcorrection2;
    QByteArray injcorrection3;
    QByteArray injcorrection4;
    QByteArray MapRef;
    QByteArray Injoverlap;
    QByteArray InjvFuel;
    QByteArray Turbotrans;
    QByteArray Oilervswater;
    QByteArray Fanvswater;
    QByteArray Fuelinj;
    QByteArray airtempboost;
    QByteArray primlagbatt;
    QByteArray AccelInj;
    QByteArray Injvsaccel;
    QByteArray Ignvsaircold;
    QByteArray notshure;
    QByteArray Ignvswater;
    QByteArray IgnvsAirwarm;

    receivedData = serialport->readAll();
    quint8 requesttype = receivedData[0];
    quint8 readwrite = receivedData[01];
    //serialport->blockSignals(true);
   // qDebug() <<  "Serial signal raised" << requesttype << readwrite;




    //serialport->blockSignals(true);
    qDebug() <<  "Serial signal raised";
    // Process to calculate checksum
    int checksum = 255; //calculated checksum from serial message 0xFF - each byte in message (except the last byte)
    QByteArray checksumhex;
    QByteArray recvchecksumhex = QByteArray::number(receivedData[readwrite], 16).right(2); // reading the checksum byte , convert to Hex , and cut to 2 positions
    recvchecksumhex = recvchecksumhex.rightJustified(2, '0'); // If the checksumbyte is less than 2 positions , prepend a 0 for example if value is 0x9 turn it into 0x09
    //test1 = test.rightJustified(2, '0');

    for (int i = 0; i <= readwrite-1; i++)
    {
        checksum = checksum - receivedData[i];
        checksumhex = QByteArray::number(checksum, 16).right(2);
        checksumhex = checksumhex.rightJustified(2, '0');
    }
    // Process to calculate checksum
    /*
     int8_t checksum = 0xFFFFFFFFFFFFFFFF;                       //calculated checksum from serial message 0xFF - each byte in message (except the last byte)
    int8_t checksumposition = readwrite;                  //determines in which position of the message the checksumbyte is located
    int8_t receivedchecksum = receivedData[checksumposition];   //checksum from serial message
    //int8_t integrity = checksum - receivedchecksum;
    for (int i = 0; i <= readwrite-1; i++)
    {
    checksum = checksum - receivedData[i];
    }
*/
    //    ui->txtConsole->append(+ "Calculated checksum " + QString::number(checksum,16).toUpper());
    //    ui->txtConsole->append(+ "received checksum " + QString::number(receivedchecksum,16).toUpper());

    // Read only Data

    //First request from FC Edit ?
    if(requesttype == 0x01 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x01 0x02 0xFC");
        ui->txtConsole->append("Sending reply FC info...First request from FC Edit ?");
        serialport->write(QByteArray::fromHex("01 03 01 fa "));
        serialport->blockSignals(false);
    }
    //FC Commander Display
    if(requesttype == 0xDA) //&& readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xDA 0x02 0x23");
        ui->txtConsole->append("Sending reply FC info...FC Commander Display");
        serialport->write(Basic);
        //serialport->write(QByteArray::fromHex("da 16 00 00 00 00 00 00 00 00 00 00 f6 02 00 00 5c 00 5b 00 7d 00 e3")); //
        serialport->blockSignals(false);
    }
    // Sensor Strings
    if(requesttype == 0xDD && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xDD 0x02 0x20");
        ui->txtConsole->append("Sending reply FC info...Sensor Strings");
        serialport->write(QByteArray::fromHex("dd 52 20 50 49 4d 56 54 41 31 56 54 41 32 56 4d 4f 50 57 54 52 54 41 49 52 54 46 55 45 4c 20 4f 32 53 53 54 52 41 2f 43 50 57 53 4e 54 52 43 4c 54 53 54 50 43 41 54 45 4c 44 48 57 4c 46 50 44 46 50 52 41 50 52 50 41 43 43 43 4e 54 43 4e 50 52 43 8b")); //
        serialport->blockSignals(false);
    }
    // PIM Strings and Injector Adjustment
    if(requesttype == 0xCB && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xCB 0x02 0x32");
        ui->txtConsole->append("Sending reply FC info...PIM Strings and Injector Adjustment");
        serialport->write(QByteArray::fromHex("cb 6d 00 20 31 2e 20 4e 6f 72 6d 61 6c 20 20 20 20 20 20 20 00 20 32 2e 20 4f 70 74 69 6f 6e 31 20 20 20 20 20 20 00 20 33 2e 20 4f 70 74 69 6f 6e 32 20 20 20 20 20 20 00 20 34 2e 20 4f 70 74 69 6f 6e 33 20 20 20 20 20 20 00 20 35 2e 20 4f 70 74 69 6f 6e 34 20 20 20 20 20 20 00 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 80 ab")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xCB && readwrite > 0x02 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write PM Strings and Inhector Adjustment to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Version number
    if(requesttype == 0xF5 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xF5 0x02 0x08");
        ui->txtConsole->append("Sending reply FC info...Version number");
        serialport->write(QByteArray::fromHex(" f5 07 34 2e 31 31 20 1f")); // Power FC Version number 5.08
        //serialport->write(QByteArray::fromHex("f6 07 34 2e 31 31 20 1f")); // Power FC Version number 5.08
        //serialport->write(QByteArray::fromHex("F507342E31302020"));

        serialport->blockSignals(false);
    }
    // Map Reference
    if(requesttype == 0x8A && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x8A 0x02 0x73");
        ui->txtConsole->append("Sending reply FC info...Map Reference");
        serialport->write(QByteArray::fromHex("8a 52 f8 0a e0 0e c8 12 b0 16 98 1a 80 1e 68 22 50 26 38 2a 20 2e 08 32 c5 33 d9 38 4c 3e b5 43 0a 49 73 4e 91 53 9b 58 6a 5c 90 01 20 03 b0 04 40 06 d0 07 60 09 f0 0a 80 0c 10 0e a0 0f 30 11 c0 12 50 14 e0 15 70 17 00 19 90 1a 20 1c b0 1d 40 1f be ")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x8A && readwrite ==0x52 && recvchecksumhex == checksumhex)
    {
        QByteArray MapRef = receivedData;
        ui->txtConsole->append("Write Map Reference to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    //Advanced data
    if(requesttype == 0xF0 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xF0 0x02 0x0D");
        ui->txtConsole->append("Sending reply FC info...Adv");
       serialport->write(Advanced);
        //serialport->write(QByteArray::fromHex("F0 20 88 13 1d 28 36 09 1d 02 00 00 00 00 00 00 ff 36 02 01 5d 5c 00 73 DC 00 00 00 00 4C 00 00 25")); //changed speed to 220 and rev to 5500
        //  serialport->write(QByteArray::fromHex("f0 28 00 00 6d 28 45 09 27 02 00 00 00 00 00 00 ff 36 02 01 5d 5c 00 7c 00 00 00 00 00 4c 00 00 00 00 00 00 00 00 00 00 22"));
        serialport->blockSignals(false);

    }
    //Sensor data
    if(requesttype == 0xDE && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xDE 0x02 0x1F");
        ui->txtConsole->append("Sending reply FC info...Sensor Info");
        serialport->write(QByteArray::fromHex("DE 14 EB 00 36 00 78 00 07 80 13 01 15 01 F2 81 01 00 28 31 F6"));
        serialport->blockSignals(false);
    }
    //Map Indicies
    if(requesttype == 0xDB && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xDB 0x02 0x22");
        ui->txtConsole->append("Sending reply FC info...Map Indicies");
        serialport->write(QByteArray::fromHex("DB 04 05 06 15"));
        serialport->blockSignals(false);
    }
    //Aux data (AN1-AN4) /lenght byte and checksum to be verified
    if(requesttype == 0x00 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x00 0x02 0xFD");
        ui->txtConsole->append("Sending reply FC info...Aux info");
        serialport->write(QByteArray::fromHex("00 06 33 66 99 CC FB"));
        serialport->blockSignals(false);
    }
    //All values below are simulating my Rx7 FD3S version 4.11 MAP
    //Read and write packets

    //This packet is sent by the Apexi Pro Software
    if(requesttype == 0xF8 && readwrite == 0x06 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xf8 0x06 0x06 0x07 0x03 0x03 0xee");
        ui->txtConsole->append("Sending reply FC info...Unknown Apexi FC Pro command");
        serialport->write(QByteArray::fromHex("f8 0a e0 0e c8 12 b0 16 98 1a "));
        serialport->blockSignals(false);
    }

    // Leading Ignition Packet 1
    if(requesttype == 0x76 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x76 0x02 0x87");
        ui->txtConsole->append("Sending reply FC info...Leading Ignition Packet1");
        serialport->write(QByteArray::fromHex("76 66 1e 1e 1e 1e 1e 22 22 1f 1d 1c 19 18 17 17 17 17 17 17 17 17 1e 1e 1e 1e 22 28 25 22 1f 1d 1b 1a 19 18 18 18 18 18 18 18 22 22 22 22 2a 2e 2a 24 22 1f 1d 1b 1a 18 18 18 18 18 18 18 2a 2a 2a 2f 2f 2f 2e 29 27 26 22 21 1e 1a 1a 19 19 19 19 19 35 35 35 34 32 30 2d 2a 28 27 26 23 21 1f 1d 1b 19 18 18 18 a6"));
        //serialport->write (Lignition1);
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x76 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Lignition1 = serialport->readAll();
        ui->txtConsole->append("Write Leading Ignition Packet 1 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Leading Ignition Packet 2
    if(requesttype == 0x77 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x77 0x02 0x86");
        ui->txtConsole->append("Sending reply FC info...Leading Ignition Packet2");
        serialport->write(QByteArray::fromHex("77 66 36 36 36 35 33 31 2c 2a 28 27 26 23 21 1f 1e 1d 1a 1a 19 19 36 36 36 36 34 32 2e 2c 29 28 26 24 23 21 20 1e 1c 1a 1a 1a 37 37 37 37 36 34 2e 2c 2b 2a 28 27 26 25 23 21 1f 1d 1b 1b 39 39 39 38 37 36 2f 2e 2b 2a 29 29 29 29 26 23 20 1e 1d 1d 3c 3c 3c 3b 3a 38 31 2f 2d 2b 2b 2b 2b 2a 28 25 22 20 1f 1d be"));
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x77 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Lignition2 = receivedData;
        ui->txtConsole->append("Write Leading Ignition Packet 2 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Leading Ignition Packet 3
    if(requesttype == 0x78 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x78 0x02 0x85");
        ui->txtConsole->append("Sending reply FC info...Leading Ignition Packet3");
        serialport->write(QByteArray::fromHex("78 66 3f 3f 3f 3e 3c 3b 34 32 2f 2e 2c 2c 2c 2b 28 26 22 20 1f 1d 41 41 41 40 3e 3d 37 33 32 31 30 30 2d 2b 28 26 21 20 1e 1c 43 43 43 42 40 3c 3a 39 37 36 33 31 2f 2c 28 26 21 1f 1d 1b 44 44 44 43 42 3e 3d 3c 39 36 35 32 2f 2d 29 26 21 1f 1d 1b 46 46 46 45 45 43 40 3e 3c 3a 36 32 2f 2d 29 25 1f 1d 1b 19 a3"));
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x78 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Lignition3 = receivedData;
        ui->txtConsole->append("Write Leading Ignition Packet 3 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Leading Ignition Packet 4
    if(requesttype == 0x79 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x79 0x02 0x84" );
        ui->txtConsole->append("Sending reply FC info...Leading Ignition Packet4");
        serialport->write(QByteArray::fromHex("79 66 47 47 47 46 46 43 41 3f 3d 3b 37 33 30 2e 2a 26 21 1f 1d 1b 47 47 47 47 46 45 41 40 3e 3d 37 33 31 2e 2b 28 24 22 20 1e 48 48 48 47 47 46 43 42 41 3f 37 34 31 2f 2d 2b 29 27 25 23 48 48 48 48 48 48 47 44 41 3f 39 36 32 2f 2e 2c 2b 29 27 25 49 49 49 49 48 48 47 44 41 3f 39 36 32 30 2f 2f 2d 2b 29 27 2a"));
     //   serialport->write(QByteArray::fromHex("79 66 47 47 47 46 46 43 41 3f 3d 3b 37 33 30 2e 2a 26 21 1f 1d 1b 47 47 47 47 46 45 41 40 3e 3d 37 33 31 2e 2b 28 24 22 20 1e 48 48 48 47 47 46 43 42 41 3f 37 34 31 2f 2d 2b 29 27 25 23 48 48 48 48 48 48 47 44 41 3f 39 36 32 2f 2e 2c 2b 29 27 25 49 49 49 49 48 48 47 44 41 3f 39 36 32 30 2f 2f 2d 2b "));
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x79 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Lignition4 = receivedData;
        ui->txtConsole->append("Write Leading Ignition Packet 4 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }

    // Trailing Ignition Packet 1
    if(requesttype == 0x81 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x81 0x02 0x7C");
        ui->txtConsole->append("Sending reply FC info...Trailing Ignition Packet1");
        serialport->write(QByteArray::fromHex("81 66 16 16 16 16 16 20 1d 19 17 16 15 0e 0d 0d 0d 0d 0d 0d 0d 0d 16 16 16 16 20 23 21 1d 1a 18 15 10 0f 0e 0e 0e 0e 0e 0e 0e 1a 1a 1a 1a 28 2d 29 23 1f 1b 19 11 10 0e 0e 0e 0e 0e 0e 0e 2a 2a 2a 2f 2f 2f 2c 28 23 21 1d 17 14 10 10 0f 0f 0f 0f 0f 35 35 35 34 32 30 2d 2a 27 25 1f 19 17 15 13 11 0f 0e 0e 0e 2d"));
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x81 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Tignition1 = receivedData;
        ui->txtConsole->append("Write Trailing Ignition Packet 1 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Trailing Ignition Packet 2
    if(requesttype == 0x82 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x82 0x02 0x7B");
        ui->txtConsole->append("Sending reply FC info...Trailing Ignition Packet 2");
        serialport->write(QByteArray::fromHex("82 66 36 36 36 35 33 31 2c 2a 27 25 20 19 17 15 14 13 10 10 0f 0f 36 36 36 36 34 32 2e 2c 28 25 20 1a 19 17 16 14 12 10 10 10 37 37 37 37 36 34 2e 2c 2a 26 21 1d 1c 1b 19 17 15 13 11 11 39 39 39 38 37 36 2f 2e 2b 27 22 1f 1f 1f 1c 19 16 14 13 13 3c 3c 3c 3b 3a 38 31 2f 2d 29 24 21 21 20 1e 1b 18 16 15 13 a7"));
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x82 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Tignition2 = receivedData;
        ui->txtConsole->append("Write Trailing Ignition Packet 2 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Trailing Ignition Packet 3
    if(requesttype == 0x83 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x83 0x02 0x7A");
        ui->txtConsole->append("Sending reply FC info...Trailing Ignition Packet 3");
        serialport->write(QByteArray::fromHex("83 66 3f 3f 3f 3e 3c 3b 34 32 2f 2c 26 22 22 21 1e 1c 18 16 15 13 41 41 41 40 3e 3d 37 33 31 2c 28 26 23 21 1e 1c 17 16 14 12 43 43 43 42 40 3c 3a 39 36 32 2b 27 25 22 1e 1c 17 15 13 11 44 44 44 43 42 3e 3d 3b 37 31 2d 28 25 23 1f 1c 17 15 13 11 46 46 46 45 44 42 3f 3c 38 34 2e 28 25 23 1f 1b 15 13 11 0f a4"));
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x83 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Tignition3 = receivedData;
        ui->txtConsole->append("Write Trailing Ignition Packet 3 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Trailing Ignition Packet 4
    if(requesttype == 0x84 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x84 0x02 0x79");
        ui->txtConsole->append("Sending reply FC info...Trailing Ignition Packet 4");
        serialport->write(QByteArray::fromHex("84 66 47 47 47 46 46 44 40 3c 3b 35 2f 29 26 24 20 1c 17 15 13 11 47 47 47 47 46 45 41 3e 3b 36 2f 29 27 24 21 1e 1a 18 16 14 48 48 48 48 47 46 43 40 3d 38 31 2a 27 25 23 21 1f 1d 1b 19 48 48 48 48 48 48 46 42 3e 39 33 2c 28 25 24 22 21 1f 1d 1b 49 49 49 49 48 48 46 42 3e 39 33 2c 28 26 25 25 23 21 1f 1d 3e"));
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x84 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray Tignition4 = receivedData;
        ui->txtConsole->append("Write Trailing Ignition Packet 4 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Init Platform

    if(requesttype == 0xF3 && readwrite <= 0x02)

    {

        ui->txtConsole->append("0xF3 0x02 0x0A");
        ui->txtConsole->append("Sending reply FC info...init platform string");
        //Simulate a delay in message reception
        //serialport->write(QByteArray::fromHex("F30a324A5A2D47544531EE")); //2JZE-GTE
        serialport->write(QByteArray::fromHex("f3 0a 31 33 42 2d 52 45 57 20 21"));  //13B-REW
/*
        serialport->write(QByteArray::fromHex("f3 0a"));
       //
        ::msleep(1000);
        serialport->write(QByteArray::fromHex("31 33"));
       // QThread::msleep(5000);
        serialport->write(QByteArray::fromHex("42 2d"));
      //  QThread::msleep(20);
        serialport->write(QByteArray::fromHex("52"));
      //  QThread::msleep(10);
        serialport->write(QByteArray::fromHex("45 57"));
      //  QThread::msleep(100);
        serialport->write(QByteArray::fromHex("20 21"));
*/


        serialport->blockSignals(false);
    }


    // Injector Overlap
    if(requesttype == 0x7B && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x7B 0x02 0x82");
        ui->txtConsole->append("Sending reply FC info...Injector Overlap");
        serialport->write(QByteArray::fromHex("7b 08 41 03 2e 02 1a 01 ed")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x7B && readwrite ==0x08 && recvchecksumhex == checksumhex)
    {
        QByteArray Injoverlap = receivedData;
        ui->txtConsole->append("Write Injector Overlap to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector vs Fuel Temp
    if(requesttype == 0x7C && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x7C 0x02 0x80");
        ui->txtConsole->append("Sending reply FC info...Injector vs Fuel Temp");
        serialport->write(QByteArray::fromHex("7c 0b 8c 05 01 78 00 01 64 00 01 08")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x7C && readwrite ==0x0B && recvchecksumhex == checksumhex)
    {
        QByteArray InjvFuel = receivedData;
        ui->txtConsole->append("Write Injector vs Fuel Temp to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }


    // Turbo Transition
    if(requesttype == 0x7D && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x7D 0x02");
        ui->txtConsole->append("Sending reply FC info...Turbo Transition");
        serialport->write(QByteArray::fromHex("7d 0b 99 66 33 4b 4b 4b 66 6b 8c 07")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x7D && readwrite ==0x0B && recvchecksumhex == checksumhex)
    {
        QByteArray Turbotrans = receivedData;
        ui->txtConsole->append("Write Turbo Transition to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Oiler vs Water Temperature
    if(requesttype == 0x7E && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x7E 0x02 0x7F");
        ui->txtConsole->append("Sending reply FC info...Oiler vs Water Temperature");
        serialport->write(QByteArray::fromHex("7e 08 be 97 aa 89 a7 81 c9")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x7E && readwrite ==0x08 && recvchecksumhex == checksumhex)
    {
        QByteArray Oilervswater = receivedData;
        ui->txtConsole->append("Write Oiler vs Water Temperature to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Fan vs Water Temperature
    if(requesttype == 0x7F && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x7F 0x02 0x7E");
        ui->txtConsole->append("Sending reply FC info...Fan vs Water Temperature");
        serialport->write(QByteArray::fromHex("7f 05 a6 a6 a6 89")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x7F && readwrite ==0x05 && recvchecksumhex == checksumhex)
    {
        QByteArray Fanvswater = receivedData;
        ui->txtConsole->append("Write Fan vs Water Temperature to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }

    // Injector Correction Packet 1
    // read request
    if(requesttype == 0x86 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x86 0x02 0x77");
        ui->txtConsole->append("Sending reply FC info...Injector Correction Packet 1");
        serialport->write(QByteArray::fromHex("86 66 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 c0")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x86 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Injector Correction Packet 1 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector Correction Packet 2
    if(requesttype == 0x87 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x87 0x02 0x76 ");
        ui->txtConsole->append("Sending reply FC info...Injector Correction Packet 2");
        serialport->write(QByteArray::fromHex("87 66 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 bf")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x87 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection2 = receivedData;
        ui->txtConsole->append("Write Injector Correction Packet 2 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector Correction Packet 3
    if(requesttype == 0x88 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x88 0x02 0x75");
        ui->txtConsole->append("Sending reply FC info...Injector Correction Packet 3");
        serialport->write(QByteArray::fromHex("88 66 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 be")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x88 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection3 = receivedData;
        ui->txtConsole->append("Write Injector Correction Packet 3 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector Correction Packet 4
    if(requesttype == 0x89 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x89 0x02 0x74");
        ui->txtConsole->append("Sending reply FC info...Injector Correction Packet 4");
        serialport->write(QByteArray::fromHex("89 66 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 80 80 80 80 80 99 99 99 99 99 99 99 99 99 99 99 99 99 99 99 bd")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x89 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection4 = receivedData;
        ui->txtConsole->append("Write Injector Correction Packet 4 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }

    // Fuel Injectors
    if(requesttype == 0x8D && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x8D 0x02 0x70 ");
        ui->txtConsole->append("Sending reply FC info...Fuel Injectors");
        serialport->write(QByteArray::fromHex("8d 1a 00 80 00 80 0a 80 23 80 0a 80 23 80 26 02 90 06 90 01 19 00 00 00 00 00 96")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x8D && readwrite ==0x1A && recvchecksumhex == checksumhex)
    {
        QByteArray Fuelinj = receivedData;
        ui->txtConsole->append("Write Fuel Injectors to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Cranking
    if(requesttype == 0x8E && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x8E 0x02 0x6F");
        ui->txtConsole->append("Sending reply FC info...Cranking");
        serialport->write(QByteArray::fromHex("8e 0e b8 0b 9a 10 58 1b aa 37 28 55 c2 65 fe")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x8E && readwrite ==0x0E && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Cranking to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Water Temp Correction
    if(requesttype == 0x8F && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x8F 0x02 0x6E");
        ui->txtConsole->append("Sending reply FC info...Water Temp Correction");
        serialport->write(QByteArray::fromHex("8f 10 40 43 4c 60 81 b4 40 41 4e 60 78 a5 1a 41 55")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x8F && readwrite ==0x10 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Cranking to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector vs Water Temperature and Boost
    if(requesttype == 0x90 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x90 0x02 0x6D");
        ui->txtConsole->append("Sending reply FC info...Injector vs Water Temperature and Boost");
        serialport->write(QByteArray::fromHex("90 08 be 8a af 80 41 1a 95")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x90 && readwrite ==0x08 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Injector vs Water Temoerature to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector vs Air Temperature and Boost
    if(requesttype == 0x91 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x91 0x02 0x6C");
        ui->txtConsole->append("Sending reply FC info...Injector vs Air Temperature and Boost");
        serialport->write(QByteArray::fromHex("91 0a aa 85 a0 82 96 80 3a 13 b0")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x91 && readwrite ==0x0A && recvchecksumhex == checksumhex)
    {
        QByteArray airtempboost = receivedData;
        ui->txtConsole->append("Write Injector vs Air Temperature to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Inj Primary Lag (uS) vs Battery Voltage
    if(requesttype == 0x92 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x92 0x02 0x6B ");
        ui->txtConsole->append("Sending reply FC info...Inj Primary Lag (uS) vs Battery Voltage");
        serialport->write(QByteArray::fromHex("92 0e 84 00 b6 00 fc 00 56 01 ec 01 bc 02 27")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x92 && readwrite ==0x0E && recvchecksumhex == checksumhex)
    {
        QByteArray primlagbatt = receivedData;
        ui->txtConsole->append("Write Injector Primary Lag  (uS) vs Battery voltage to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Accelerate Inject (mS)
    if(requesttype == 0x93 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x93 0x02 0x6A ");
        ui->txtConsole->append("Sending reply FC info...Accelerate Inject (mS)");
        serialport->write(QByteArray::fromHex("93 1b 7d 64 4b 32 19 53 07 53 07 53 07 d0 07 dc 05 fa 00 fa 00 77 01 c2 01 f4 01 f0")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x93 && readwrite ==0x1B && recvchecksumhex == checksumhex)
    {
        QByteArray AccelInj = receivedData;
        ui->txtConsole->append("Write Accelerate Inject (mS) to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector vs Accel TPS
    if(requesttype == 0x94 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x94 0x02 0x69");
        ui->txtConsole->append("Sending reply FC info...Injector vs Accel TPS");
        serialport->write(QByteArray::fromHex("94 0b 67 00 01 25 e6 00 14 19 00 c0")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x94 && readwrite ==0x0B && recvchecksumhex == checksumhex)
    {
        QByteArray Injvsaccel = receivedData;
        ui->txtConsole->append("Write Injector vs Accel to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Ignition vs Air Temperature (cold)
    if(requesttype == 0x96 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x96 0x02 0x67");
        ui->txtConsole->append("Sending reply FC info...Ignition vs Air Temperature (cold)");
        serialport->write(QByteArray::fromHex("96 06 91 00 8c 00 46")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x96 && readwrite ==0x06 && recvchecksumhex == checksumhex)
    {
        QByteArray Ignvsaircold = receivedData;
        ui->txtConsole->append("Write Ignition vs Air Temperature (cold) to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }

    // not shure what this is
    if(requesttype == 0x97 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x9 0x02 ");
        ui->txtConsole->append("Sending reply FC info...not shure what this is ");
        serialport->write(QByteArray::fromHex("97 04 31 26 0d")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x97 && readwrite ==0x04 && recvchecksumhex == checksumhex)
    {
        QByteArray notshure = receivedData;
        ui->txtConsole->append("Write not shure what this is to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Ignition vs Water Temperature
    if(requesttype == 0x98 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x98 0x02 0x65");
        ui->txtConsole->append("Sending reply FC info...Ignition vs Water Temperature");
        serialport->write(QByteArray::fromHex("98 06 b4 06 af 00 f8")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x98 && readwrite ==0x06 && recvchecksumhex == checksumhex)
    {
        QByteArray Ignvswater = receivedData;
        ui->txtConsole->append("Write Ignition vs Water Temperature to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Ignition vs Air Temperature (warm)
    if(requesttype == 0x9A && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x9A 0x02 0x63");
        ui->txtConsole->append("Sending reply FC info...Ignition vs Air Temperature (warm)");
        serialport->write(QByteArray::fromHex("9a 08 96 06 8c 04 82 00 af")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x9A && readwrite ==0x08 && recvchecksumhex == checksumhex)
    {
        QByteArray IgnvsAirwarm = receivedData;
        ui->txtConsole->append("Write Ignition vs Air Temperature (warm) to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Leading Ignition vs RPM
    if(requesttype == 0x9B && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x9B 0x02 0x62");
        ui->txtConsole->append("Sending reply FC info...Leading Ignition vs RPM");
        serialport->write(QByteArray::fromHex("9b 08 7b 6c 46 22 11 09 f3")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x9B && readwrite ==0x08 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Leading Ignition vs RPM to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // IGN vs Battery Voltage
    if(requesttype == 0x9C && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x9C 0x02 0x61");
        ui->txtConsole->append("Sending reply FC info...IGN vs Battery Voltage");
        serialport->write(QByteArray::fromHex("9c 08 35 4051 67 77 87 30")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x9C && readwrite ==0x08 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write IGN vs Battery Voltage to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Boost vs Ignition S.F.
    if(requesttype == 0x9D && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x9D 0x02 0x60");
        ui->txtConsole->append("Sending reply FC info...Boost vs Ignition S.F.");
        serialport->write(QByteArray::fromHex("9d 06 64 32 26 16 8a")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x9D && readwrite ==0x06 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Boost vs Ignition S.F to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Trailing Ignition vs RPM
    if(requesttype == 0x9E && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x9E 0x02 0x5F");
        ui->txtConsole->append("Sending reply FC info...Trailing Ignition vs RPM");
        serialport->write(QByteArray::fromHex("9e 08 7b 78 4b 23 11 09 de")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x9E && readwrite ==0x08 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Trailing Ignition vs RPM to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector Secondary Lag (uS) vs Battery Voltage
    if(requesttype == 0x9F && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0x9F 0x02 0x5E");
        ui->txtConsole->append("Sending reply FC info...Injector Secondary Lag (uS) vs Battery Voltage");
        serialport->write(QByteArray::fromHex("9f 0e 8e 00 c0 00 06 01 60 01 f6 01 c6 02 dd")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0x9F && readwrite ==0x0E && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Secondary Lag (uS) to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector Warning
    if(requesttype == 0xA8 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xA8 0x02 0x55");
        ui->txtConsole->append("Sending reply FC info...Injector Warning");
        serialport->write(QByteArray::fromHex("a8 06 00 d4 03 1e 5c")); //

        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xA8 && readwrite ==0x06 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Injector warning to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Knock Warning
    if(requesttype == 0xA9 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xA9 0x02 ");
        ui->txtConsole->append("Sending reply FC info...Knock Warning");
        serialport->write(QByteArray::fromHex("a9 06 00 3c 00 09 0b")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xA9 && readwrite ==0x06 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Knock warning to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // O2 Feedback
    if(requesttype == 0xAA && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xAA 0x02 0x53");
        ui->txtConsole->append("Sending reply FC info...O2 Feedback");
        serialport->write(QByteArray::fromHex("aa 05 ff 0c 01 44")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xAA && readwrite ==0x05 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write 02 Feedback to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Boost control
    if(requesttype == 0xAB && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xAB 0x02 0x52");
        ui->txtConsole->append("Sending reply FC info...Boost control");
        serialport->write(QByteArray::fromHex("ab 0d ff 00 00 12 12 12 12 28 1e 32 19 6f")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xAB && readwrite ==0x0D && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Boost control to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Setting Protections?
    if(requesttype == 0xAC && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xAC 0x02 0x51");
        ui->txtConsole->append("Sending reply FC info...Setting Protections?");
        serialport->write(QByteArray::fromHex("ac 0c 00 00 00 00 00 00 00 00 00 00 47")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xAC && readwrite ==0x0C && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Setting Protections to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Tuner String?
    if(requesttype == 0xAD && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xAD 0x02 0x50");
        ui->txtConsole->append("Sending reply FC info...Tuner String?");
        serialport->write(QByteArray::fromHex("ad 0a 20 20 20 20 20 20 20 20 48")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xAD && readwrite ==0x0A && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Tuner String to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Base Fuel packet 1
    if(requesttype == 0xB0 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB0 0x02 0x4D ");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 1");
        serialport->write(QByteArray::fromHex("b0 66 60 00 c1 00 32 01 ca 01 74 02 62 03 1b 04 a3 04 54 05 60 06 44 07 ef 07 a9 08 4b 09 f3 09 7c 0a e6 0b 49 0d bd 0e 44 10 64 00 c8 00 4c 01 f5 01 88 02 7c 03 32 04 bd 04 5e 05 7e 06 76 07 12 08 bf 08 72 09 12 0a b9 0a 15 0c 8c 0d 14 0f 98 10 5e 00 be 00 4b 01 f2 01 39 02 fd 02 93 03 1f 04 de 04 fb 05 79")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB0 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 1 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Base Fuel packet 2
    if(requesttype == 0xB1 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB1 0x02 0x4C");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 2");
        serialport->write(QByteArray::fromHex("b1 66 e1 06 67 07 19 08 cf 08 67 09 f7 09 36 0b 71 0c d9 0d 40 0f 5c 00 b5 00 4d 01 d7 01 5b 02 12 03 af 03 49 04 b4 04 25 06 0e 07 b2 07 69 08 30 09 cd 09 76 0a c2 0b ee 0c 63 0e e9 0f 5f 00 cd 00 58 01 08 02 7e 02 2b 03 cc 03 63 04 f4 04 2a 06 0e 07 cc 07 86 08 61 09 01 0a c4 0a 3d 0c 5f 0d b6 0e 6d 10 65")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB1 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 2 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Base Fuel packet 3
    if(requesttype == 0xB2 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB2 0x02 0x4B");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 3");
        serialport->write(QByteArray::fromHex("b2 66 63 00 df 00 74 01 29 02 ad 02 53 03 e7 03 89 04 19 05 6d 06 3a 07 d4 07 ad 08 a8 09 47 0a 16 0b a6 0c 03 0e 68 0f 2f 11 70 00 04 01 81 01 40 02 c5 02 66 03 fb 03 b1 04 61 05 9a 06 5c 07 ef 07 ea 08 bd 09 e7 0a bd 0b bd 0d 3c 0f fb 10 db 12 83 00 1c 01 8d 01 3e 02 c1 02 60 03 05 04 ff 04 e7 05 dc 06 23")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB2 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 3 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Base Fuel packet 4
    if(requesttype == 0xB3 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB3 0x02 0x4A");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 4");
        serialport->write(QByteArray::fromHex("b3 66 ad 07 2e 08 1d 09 fd 09 61 0b a9 0c 24 0f 17 11 18 13 45 15 8a 00 2c 01 a9 01 4a 02 09 03 ad 03 6d 04 0e 05 e0 05 d1 06 b4 07 36 08 36 09 24 0a 8d 0b a1 0c 7c 0f bb 11 f2 13 50 16 8f 00 35 01 b4 01 63 02 1a 03 aa 03 91 04 32 05 0f 06 f7 06 b0 07 8d 08 71 09 76 0a ba 0b db 0c b8 0f 20 12 5b 14 ca 16 f8")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB3 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 4 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Base Fuel packet 5
    if(requesttype == 0xB4 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB4 0x02 0x49");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 5");
        serialport->write(QByteArray::fromHex("b4 66 91 00 39 01 aa 01 62 02 0c 03 cb 03 d2 04 7b 05 5c 06 31 07 33 08 e1 08 0c 0a 5a 0b 85 0c ca 0d 10 10 85 12 de 14 88 17 98 00 47 01 c7 01 60 02 30 03 0d 04 e8 04 a5 05 8d 06 80 07 9f 08 56 09 5a 0a e5 0b 20 0d 42 0e 99 10 21 13 96 15 25 18 a4 00 62 01 fa 01 8b 02 78 03 90 04 6b 05 1f 06 f3 06 f3 07 2f")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB4 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 5 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    //Base Fuel packet 6
    if(requesttype == 0xB5 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB5 0x02 0x48");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 6");
        serialport->write(QByteArray::fromHex("b5 66 fb 08 aa 09 ae 0a 66 0c 6f 0d 7d 0e e6 10 54 14 db 16 50 19 9d 00 57 01 cf 01 ae 02 a3 03 d8 04 bc 05 91 06 79 07 4e 08 75 09 2a 0a 09 0b 6b 0c 81 0d 6e 0e 0b 11 91 14 12 17 72 19 9d 00 57 01 ca 01 9f 02 a1 03 d3 04 c9 05 a4 06 ac 07 bf 08 fa 09 b7 0a 78 0b 97 0c ad 0d f6 0e 89 11 a5 14 55 17 c5 19 51")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB5 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 6 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    //Base Fuel packet 7
    if(requesttype == 0xB6 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB6 0x02 0x47");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 7");
        serialport->write(QByteArray::fromHex("b6 66 9c 00 51 01 bf 01 99 02 a2 03 ce 04 b0 05 84 06 be 07 dc 08 e4 09 a8 0a af 0b c7 0c bc 0d a3 0e 5d 11 46 14 81 16 cc 18 91 00 4a 01 a0 01 6f 02 96 03 b6 04 95 05 66 06 a9 07 d4 08 a6 09 83 0a 9a 0b b5 0c 9e 0d 42 0e ff 10 21 13 06 15 d8 16 82 00 28 01 7e 01 44 02 75 03 73 04 79 05 46 06 49 07 a8 08 6d")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB6 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 7 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Base Fuel packet 8
    if(requesttype == 0xB7 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB7 0x02 0x46");
        ui->txtConsole->append("Sending reply FC info...Base Fuel packet 8");
        serialport->write(QByteArray::fromHex("b7 66 64 09 3d 0a 6a 0b 58 0c 61 0d db 0d 5b 10 1e 12 92 13 04 15 72 00 08 01 5d 01 33 02 3d 03 2c 04 3c 05 e5 05 e2 06 37 08 0f 09 a7 09 d2 0a ae 0b b5 0c 52 0d 96 0f 2d 11 48 12 a3 13 60 00 e6 00 41 01 e6 01 eb 02 ea 03 e9 04 a8 05 96 06 90 07 49 08 f3 08 10 0a e3 0a fe 0b 8c 0c b0 0e 1d 10 b9 10 22 12 74")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB7 && readwrite ==0x66 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Base Fuel packet 8 to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Rev Idle
    if(requesttype == 0xB8 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB8 0x02 0x45");
        ui->txtConsole->append("Sending reply FC info...Rev Idle");
        serialport->write(QByteArray::fromHex("b8 10 a4 1f e8 03 4c 04 b0 04 d0 02 20 03 84 03 09")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB8 && readwrite ==0x10 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write REV idle to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector vs Air Temperature
    if(requesttype == 0xB9 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xB9 0x02 0x44");
        ui->txtConsole->append("Sending reply FC info...Injector vs Air Temperature");
        serialport->write(QByteArray::fromHex("b9 0e f5 00 fc 00 03 01 0e 01 1a 01 26 01 f2")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xB9 && readwrite ==0x0E && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Injector vs Air Temperature to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Injector vs TPS
    if(requesttype == 0xBA && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xBA 0x02 0x43");
        ui->txtConsole->append("Sending reply FC info...Injector vs TPS");
        serialport->write(QByteArray::fromHex("ba 0e e6 80 cc 80 b3 80 99 80 80 80 66 80 53")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xBA && readwrite ==0x0E && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Injector vs TPS to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Ignition vs TPS
    if(requesttype == 0xBB && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xBB 0x02 0x42");
        ui->txtConsole->append("Sending reply FC info...Ignition vs TPS");
        serialport->write(QByteArray::fromHex("bb 0a cc 80 99 80 66 80 33 80 3c")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xBB && readwrite ==0x0A && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Ignition vs TPS to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // PIM Scale and Offset
    if(requesttype == 0xBC && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xBC 0x02 0x41");
        ui->txtConsole->append("Sending reply FC info...PIM Scale and Offset");
        serialport->write(QByteArray::fromHex("bc 16 0e 89 5b 08 54 ab d9 0c ee b5 d8 15 ee b5 d8 15 ee b5 d8 15 9f")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xBC && readwrite == 0x16 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write PIM Scale and Offset to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Warning and Control Strings and Flags
    if(requesttype == 0xD6 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xD6 0x02 0x27");
        ui->txtConsole->append("Sending reply FC info...Warning and Control Strings and Flags");
        serialport->write(QByteArray::fromHex("d6 57 ff 00 00 ff 00 31 2e 53 65 71 2e 54 75 72 62 6f 20 43 6f 6e 74 32 2e 49 6e 6a 65 63 74 6f 72 20 57 61 72 6e 2e 33 2e 4b 6e 6f 63 6b 20 57 61 72 6e 69 6e 67 20 34 2e 4f 32 20 46 2f 42 20 43 6f 6e 74 72 6f 6c 35 2e 49 64 6c 65 2d 49 47 20 43 6e 74 72 6c 20 d1")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xD6 && readwrite == 0x07 && recvchecksumhex == checksumhex)
    {
        QByteArray injcorrection1 = receivedData;
        ui->txtConsole->append("Write Warning and Control Strings and Flags to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Not documented ???
    if(requesttype == 0xF4 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xF4 0x02 0x09");
        ui->txtConsole->append("Sending reply FC info...Not documented ???");
        serialport->write(QByteArray::fromHex("f4 03 02 06")); //
        serialport->blockSignals(false);
    }
    //write request
    if(requesttype == 0xF4 && readwrite ==0x03 && recvchecksumhex == checksumhex)
    {
        QByteArray ndocumented = receivedData;
        ui->txtConsole->append("Write Not documented ??? to PowerFC");
        ui->txtConsole->append("Power FC Send Acknlowedged 0xF2 0x02 0x0B");
        serialport->write(QByteArray::fromHex("F2 02 0B"));
        serialport->blockSignals(false);
    }
    // Not documented ???
    if(requesttype == 0xBD && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xBD 0x02 0x40");
        ui->txtConsole->append("Sending reply FC info...Not documented ???");
        serialport->write(QByteArray::fromHex("BD 03 00 3F")); //
        serialport->blockSignals(false);
    }
    // Not documented ???
    if(requesttype == 0xBE && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xBE 0x02 0x3F");
        ui->txtConsole->append("Sending reply FC info...Not documented ???");
        serialport->write(QByteArray::fromHex("BE 03 00 3E")); //
        serialport->blockSignals(false);
    }
    // Not documented ???
    if(requesttype == 0xD8 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xD8 0x02 0x");
        ui->txtConsole->append("Sending reply FC info...Not documented ???");
        serialport->write(QByteArray::fromHex("D8 03 2D F7")); //
        serialport->blockSignals(false);
    }
    // Not documented ???
    if(requesttype == 0xD9 && readwrite <= 0x02 && recvchecksumhex == checksumhex)
    {
        ui->txtConsole->append("0xD9 0x02 0x");
        ui->txtConsole->append("Sending reply FC info...Not documented ???");
        serialport->write(QByteArray::fromHex("D9 03 14 0F")); //
        serialport->blockSignals(false);

    }
    if (requesttype == 05) // Dicktator read maps and settings from ECU
    {
        ui->txtConsole->append("reply to request 05 f8 01 60 ea48");
        serialport->write(QByteArray::fromHex("53 54 41 4c 4c 84 03 b5 04 14 05 78 05 0e 06 3a 07 f6 09 ac 0d 42 0e 9a 10 88 13 76 16 64 19 58 1b 4c 1d 45 1f 34 21 8e 03 b5 04 14 05 78 05 0e 06 3a 07 f6 09 ac 0d 42 0e 9a 10 88 13 76 16 64 19 58 1b 4c 1d 45 1f 34 21 84 03 0b 04 e2 04 78 05 0e 06 3a 07 f6 09 ac 0d 42 0e 9a 10 88 13 76 16 64 19 58 1b 4c 1d 45 1f 34 21 84 03 0b 04 e2 04 78 05 0e 06 3a 07 f6 09 ac 0d 42 0e 9a 10 88 13 76 16 64 19 58 1b 4c 1d 45 1f 34 21 e8 03 51 04 78 05 78 05 0e 06 3a 07 f0 0a ac 0d a0 0f 94 11 88 13 7c 15 64 19 58 1b 4c 1d 45 1f 34 21 e8 03 51 04 78 05 a0 05 0e 06 3a 07 f0 0a ac 0d a0 0f 94 11 88 13 7c 15 64 19 58 1b 4c 1d 45 1f 34 21 e8 03 51 04 78 05 a0 05 0e 06 3a 07 f0 0a ac 0d a0 0f 94 11 88 13 7c 15 64 19 58 1b 4c 1d 45 1f 34 21 e8 03 51 04 78 05 a0 05 0e 06 3a 07 f0 0a ac 0d a0 0f 94 11 88 13 7c 15 64 19 58 1b 4c 1d 45 1f 34 21 e8 03 51 04 78 05 a0 05 0e 06 3a 07 f0 0a ac 0d a0 0f 94 11 88 13 7c 15 64 19 58 1b 4c 1d 45 1f 34 21 e8 03 51 04 78 05 a0 05 0e 06 3a 07 f0 0a ac 0d a0 0f 94 11 88 13 7c 15 64 19 58 1b 4c 1d 45 1f 34 21 e8 03 51 04 78 05 a0 05 0e 06 3a 07 f0 0a ac 0d a0 0f 94 11 88 13 7c 15 64 19 58 1b 4c 1d 45 1f 1d 20 10 8e 8e 8e 8e 8e 8a 88 84 84 84 82 82 84 84 88 7a 7a 80 84 88 70 78 7e 82 88 70 78 7c 80 88 70 78 7c 80 88 70 78 7c 80 88 70 78 7c 80 88 70 78 7c 80 88 70 78 7c 80 88 89 16 83 82 02 00 00 07 40 06 03 6b 0a 36 00 00 20 03 40 cb d4 44 41 01 01 80 80 80 80 80 80 80 80 f2 dd c6 99 6a 53 38 2a 9a 74 4d 27 00 00 00 00 e5 c1 95 60 38 23 13 07 82 3d 45 1b a0 0f af 96 7d 64 00 00 00 00 00 00 20 4e 00 00 f8 79 cc 33 44 4f 4e 45 41"));
    }
    else
    {
      ui->txtConsole->append(receivedData.toHex());
    }
}






void MainWindow::on_btnDisconnect_clicked()
{
    this->ui->btnconnect->setDisabled(false);
    serialport->close();
    ui->txtConsole->append("closed Serialport");
   // qDebug() << "Connection closed.";
}



void MainWindow::on_btnconnect_clicked()
{
    this->ui->btnconnect->setDisabled(true);

    serialport = new QSerialPort(this);
    serialport->setPortName(ui->comboBox->currentText());
    serialport->setBaudRate(QSerialPort::Baud19200);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setParity(serialport->NoParity);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
    serialport->open(QIODevice::ReadWrite);
    //qDebug() << "Serialport initialized"<< ui->comboBox->currentText();
    connect(this->serialport,SIGNAL(readyRead()),this,SLOT(dataAvailable()));
    ui->txtConsole->append("Serialport status  "+serialport->errorString() );

}
/*
void MainWindow::writeRequestPFC(QByteArray p_reply)
{
   // qDebug() << "write request" << p_reply.toHex();
    m_replyData = p_reply;
    qint64 bytesWritten = m_serialport->write(p_reply);

    //Action to be implemented
    if (bytesWritten == -1) {
        //m_dashBoard->setSerialStat(m_serialport->errorString());
        qDebug() << "Write request to port failed" << (m_serialport->errorString());
    } else if (bytesWritten != m_writeData.size()) {
       // m_dashBoard->setSerialStat(m_serialport->errorString());
        qDebug() << "could not write complete request to port" << (m_serialport->errorString());
    }
}
*/

void MainWindow::dicktator()
{
    //qDebug() << "Sending Live Data " ;
    serialport->write(QByteArray::fromHex("53 54 41 52 54 00 ff 01 00 80 15 06 b3 46 a6 00 0D 80 c3 ff 00 00 09 0f 94 ff 11 00 01 00 45 4e 44"));
                                          //------------- -RPM---- -PoT- -FUEL -Tim- -BST- wt ai bt -TPS-             I% s1 s2 s3 s4 --------

}

void MainWindow::on_pushButton_clicked()
{

    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(dicktator()));
        timer->start(20); //time specified in ms

}




void MainWindow::on_SpeedSlider_sliderMoved(int position)
{
    //qDebug() << "Slider Position " << ui->SpeedSlider->sliderPosition();
    int Speed = ui->SpeedSlider->sliderPosition();
    //qDebug() << "Speed " << Speed;
    int checksum = 255; //calculated checksum from serial message 0xFF - each byte in message (except the last byte)
    int newchecksum;
    QByteArray checksumhex;
    //Advanced Data
    //qDebug() << "Advanced old" << Advanced.toHex();
    Advanced.remove(24,2);
    Advanced.insert(24,((char)(Speed & 0x00FF)));
    Advanced.insert(25,((char)((Speed & 0xFF00) >> 8)));
    //Recalc Checksum for advanced
    quint8 readwrite = Advanced[01];
    for (int i = 0; i <= readwrite-1; i++)
    {
        checksum = checksum - Advanced[i];
        checksumhex = QByteArray::number(checksum, 16).right(2);
        checksumhex = checksumhex.rightJustified(2, '0');
    }
    //qDebug() << "new Advanced checksum " << checksumhex;
    newchecksum = checksumhex.toInt();
//    Advanced.remove(33,1);
    //Advanced.append(checksumhex);
    //qDebug() << "Advanced new " << Advanced.toHex();
    //Basic Data
    Basic.remove(10,2);
    Basic.insert(10,((char)(Speed & 0x00FF)));
    Basic.insert(11,((char)((Speed & 0xFF00) >> 8)));
    //Recalc Checksum for Basic
    readwrite = Basic[01];
    for (int i = 0; i <= readwrite-1; i++)
    {
        checksum = checksum - Basic[i];
        checksumhex = QByteArray::number(checksum, 16).right(2);
        checksumhex = checksumhex.rightJustified(2, '0');
    }
    //qDebug() << "new basic checksum " << checksumhex;
    newchecksum = checksumhex.toInt();
    //qDebug() << "new basic int " << newchecksum;
    //qDebug() << "old basic checksum " << Basic.toHex();
//    Basic.remove(22,1);
//    Basic.append(checksumhex.toHex());
    //qDebug() << "new basic " << Basic.toHex();
}

void MainWindow::on_REVSlider_sliderMoved(int position)
{
    //qDebug() << "Slider Position " << ui->REVSlider->sliderPosition();
    int REV = ui->REVSlider->sliderPosition()*100;
    //qDebug() << "REV " << REV;
    int checksum = 255; //calculated checksum from serial message 0xFF - each byte in message (except the last byte)
    int newchecksum;
    QByteArray checksumhex;
    //Advanced Data
    //qDebug() << "Advanced old" << Advanced.toHex();
    Advanced.remove(2,2);
    Advanced.insert(2,((char)(REV & 0x00FF)));
    Advanced.insert(3,((char)((REV & 0xFF00) >> 8)));
    //Recalc Checksum for advanced
    quint8 readwrite = Advanced[01];
    for (int i = 0; i <= readwrite-1; i++)
    {
        checksum = checksum - Advanced[i];
        checksumhex = QByteArray::number(checksum, 16).right(2);
        checksumhex = checksumhex.rightJustified(2, '0');
    }
    //qDebug() << "new Advanced checksum " << checksumhex;
    newchecksum = checksumhex.toInt();
//    Advanced.remove(33,1);
    //Advanced.append(checksumhex);
    //qDebug() << "Advanced new " << Advanced.toHex();
    //Basic Data
    Basic.remove(8,2);
    Basic.insert(8,((char)(REV & 0x00FF)));
    Basic.insert(9,((char)((REV & 0xFF00) >> 8)));
    //Recalc Checksum for Basic
    readwrite = Basic[01];
    for (int i = 0; i <= readwrite-1; i++)
    {
        checksum = checksum - Basic[i];
        checksumhex = QByteArray::number(checksum, 16).right(2);
        checksumhex = checksumhex.rightJustified(2, '0');
    }
    //qDebug() << "new basic checksum " << checksumhex;
    newchecksum = checksumhex.toInt();
    //qDebug() << "new basic int " << newchecksum;
    //qDebug() << "old basic checksum " << Basic.toHex();
//    Basic.remove(22,1);
//    Basic.append(checksumhex.toHex());
    //qDebug() << "new basic " << Basic.toHex();

}
