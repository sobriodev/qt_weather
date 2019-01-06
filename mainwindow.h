#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QDate>
#include <QJsonDocument>
#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include "thread.h"
#include "parser.h"
#include "sensordata.h"
#include "datastorage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const QString SendAllCommand;
    static const int DefaultRefresh = 500;
    static const QString JsonFilename;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTimerTimeout(int timeout);

private:
    void configureSerialPort();
    void connectHandlers();
    void loadJsonData();

private slots:
    void onThreadTick();
    void onTimerTimeout();

    /* Data readout handlers */
    void onTemperatureChanged(double temperature);
    void onHumidityChanged(double humidity);
    void onPressureChanged(double pressure);
    void onSensorDataChanged(const SensorData data);

private:
    Ui::MainWindow *ui;
    Thread thread;
    QSerialPort serial;
    QByteArray dataRead;
    QByteArray dataWrite;
    DataStorage storage;
    QJsonDocument jsonDocument;
    Parser parser;
    QTimer timer;
    QDate date;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
