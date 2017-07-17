#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void fillPortsParameters();
    void on_btnConnect_clicked();


public:

QSerialPort *serialport;



public slots:

void dataAvailable();

private slots:
void on_btnDisconnect_clicked();
void on_btnconnect_clicked();
void on_pushButton_clicked();
void dicktator();
void on_SpeedSlider_sliderMoved(int position);
void on_REVSlider_sliderMoved(int position);
};
#endif // MAINWINDOW_H
