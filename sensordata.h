#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>

class SensorData : public QObject
{
    Q_OBJECT
public:
    explicit SensorData(QObject *parent = nullptr);
    SensorData(double temperature, double humidity, double pressure, QObject *parent = nullptr);

    double getTemperature() const;
    void setTemperature(double value);

    double getHumidity() const;
    void setHumidity(double value);

    double getPressure() const;
    void setPressure(double value);

    QString toString() const;

private:
    double temperature;
    double humidity;
    double pressure;
    // TODO add other fields
};

#endif // SENSORDATA_H
