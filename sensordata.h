#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QDate>
#include <QJsonObject>
#include <QObject>

class SensorData
{
public:
    static const QString DateFormat;
    static const QString TimeFormat;

    explicit SensorData();
    SensorData(double temperature, double humidity, double pressure);
    bool operator==(const SensorData &other) const;

    /* Basic measurements */
    double getTemperature() const;
    void setTemperature(double value);
    double getHumidity() const;
    void setHumidity(double value);
    double getPressure() const;
    void setPressure(double value);
    QDateTime getDateTime() const;
    void setDateTime(const QDateTime &value);

    /* Extendend measurements */
    double calcCloudiness() const;
    double calcPerseivedTemperature() const;
    bool calcBioConds() const;
    QPair<double, bool> calcLunarProperties() const;

    /* Serialization/deserialization */
    void serialize(QJsonObject &output) const;
    void deserialize(const QJsonObject &input);
    static SensorData fromJson(const QJsonObject &input);

    QString toString() const;

private:
    double temperature;
    double humidity;
    double pressure;
    QDateTime dateTime;
};

#endif // SENSORDATA_H
