#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>

/*!
 * \class MainWindow
 * \inmodule core
 * \brief The MainWindow class joins the GUI and the application logic
 */

/*!
 * \variable MainWindow::DefaultRefresh
 * \brief The variable stores default USB data refresh time
 */

/*!
 * \variable MainWindow::SendAllCommand
 * \brief Default command for sending all sensor data
 */
const QString MainWindow::SendAllCommand = "SD";

/*!
 * \variable MainWindow::JsonFilename
 * \brief Default JSON filename
 */
const QString MainWindow::JsonFilename = "data.json";

/*!
 * \brief MainWindow constructor with optional \a parent widget
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectHandlers();
    configureSerialPort();
    loadJsonData();
}

/*!
 * \brief MainWindow destructor
 */
MainWindow::~MainWindow()
{
    thread.terminate();
    thread.wait();
    serial.close();
    delete ui;
}

/*!
 * \brief Set sensor data readout frequency. It is used for choosing USB data refresh time. The \a timeout parameter stores new timeout in milliseconds
 */
void MainWindow::setTimerTimeout(int timeout)
{
    timer.stop();
    timer.start(timeout);
}

/*!
 * \internal
 * \brief Find correct NXP device and prepare data broadcast
 */
void MainWindow::configureSerialPort()
{
    ui->statusBar->showMessage(tr("Device not found"));
    QString portname;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        if (info.serialNumber() == "NXP-77") {
            portname=info.portName();
            serial.setPortName(portname);
            if (serial.open(QIODevice::ReadWrite)) {
                serial.setBaudRate(serial.Baud115200,  serial.AllDirections);
                ui->statusBar->showMessage(tr("Device: %1").arg(info.serialNumber()));
                serial.clear();
                thread.start(thread.HighestPriority);
                timer.start(DefaultRefresh);
            } else {
                ui->statusBar->showMessage(tr("Can't open %1, error code %2") .arg(serial.portName()).arg(serial.error()));
                return;
            }
            break;
        }
    }
}

/*!
 * \internal
 * \brief Connect all signals with handlers
 */
void MainWindow::connectHandlers()
{
    /* External thread tick */
    connect(&thread, &Thread::tick, this, &MainWindow::onThreadTick);

    /* Sensor data */
    connect(&parser, &Parser::temperatureChanged, this, &MainWindow::onTemperatureChanged);
    connect(&parser, &Parser::pressureChanged, this, &MainWindow::onPressureChanged);
    connect(&parser, &Parser::humidityChanged, this, &MainWindow::onHumidityChanged);
    connect(&parser, &Parser::allDataChanged, this, &MainWindow::onSensorDataChanged);

    /* Timer */
    connect(&timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
}

void MainWindow::loadJsonData()
{
    QFile jsonFile(JsonFilename);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot load JSON data";
    } else {
        QByteArray jsonData = jsonFile.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        if (jsonDoc.isEmpty())
            qDebug() << "An Empty/invalid JSON";

        storage.deserialize(jsonDoc.object());
    }
}

/*!
 * \internal
 * \brief External thread tick handler. It is used for checking USB data presence
 */
void MainWindow::onThreadTick()
{
    if (serial.size()) {
        parser.parseSensorData(serial.readAll());
    }
}

/*!
 * \internal
 * \brief Timer timeout handler. It sends USB commands
 */
void MainWindow::onTimerTimeout()
{
    dataWrite.clear();
    dataWrite.append(SendAllCommand);
    serial.write(dataWrite);
}

/*!
 * \internal
 * \brief Temperature changed slot. This is used for presenting and storing actual values. The \a temperature parameter holds new value
 */
void MainWindow::onTemperatureChanged(double temperature)
{
    Q_UNUSED(temperature);
}

/*!
 * \internal
 * \brief Humidity changed slot. This is used for presenting and storing actual values. The \a humidity parameter holds new value
 */
void MainWindow::onHumidityChanged(double humidity)
{
    Q_UNUSED(humidity)
}

/*!
 * \internal
 * \brief Pressure changed slot. This is used for presenting and storing actual values. The \a pressure parameter holds new value
 */
void MainWindow::onPressureChanged(double pressure)
{
    Q_UNUSED(pressure)
}

void MainWindow::onSensorDataChanged(const SensorData data)
{
    storage.append(data);
    QJsonObject obj;
    storage.serialize(obj);
    qDebug() << obj;
    storage.deleteOldReadouts(0, QTime(0, 2, 0));
}

/*!
 * \brief Overrided closeEvent function used to save JSON data. The \a event arguement carries event data
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QFile jsonFile(JsonFilename);
    if (!jsonFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot write JSON data";
    } else {
        qDebug() << "Saving JSON data";

        QJsonObject jsonObj;
        storage.serialize(jsonObj);
        QJsonDocument jsonDoc(jsonObj);
        jsonFile.resize(0);
        jsonFile.write(jsonDoc.toJson());
    }

    event->accept();
}
