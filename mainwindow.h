#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include "thread.h"
#include "parser.h"
#include "sensordata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const QString SendAllCommand;
    static const int DefaultRefresh = 1000;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTimerTimeout(int timeout);

private:
    void configureSerialPort();
    void connectHandlers();

private slots:
    void onThreadTick();
    void onTimerTimeout();

    /* Data readout handlers */
    void onTemperatureChanged(double temperature);
    void onHumidityChanged(double humidity);
    void onPressureChanged(double pressure);

private:
    Ui::MainWindow *ui;
    Thread thread;
    QSerialPort serial;
    QByteArray dataRead;
    QByteArray dataWrite;
    SensorData sensorData;
    Parser parser;
    QTimer timer;

};

#endif // MAINWINDOW_H
