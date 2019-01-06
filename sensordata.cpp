#include "sensordata.h"
#include <QDebug>

/*!
 * \class SensorData
 * \inmodule core
 * \brief The SensorData class represents single data measurment
 */

/*!
 * \variable SensorData::DateFormat
 * \brief Date format used in application
 */
const QString SensorData::DateFormat = "dd-MM-yyyy";

/*!
 * \variable SensorData::TimeFormat
 * \brief Time format used in application
 */
const QString SensorData::TimeFormat = "hh:mm:ss";

/*!
 * \brief The default SensorData constructor sets all sensor data to zero.
 */
SensorData::SensorData()
    : temperature(0), humidity(0), pressure(0), dateTime(QDateTime::currentDateTime()) {}

/*!
 * \brief The Sensor data constructor initializes all values using \a temperature, \a humidity and \a pressure arguments.
 */
SensorData::SensorData(double temperature, double humidity, double pressure)
    : temperature(temperature), humidity(humidity), pressure(pressure), dateTime(QDateTime::currentDateTime()) {}

/*!
 * \brief Equality operator. It returns true if \a other date is the same, false otherwise
 */
bool SensorData::operator==(const SensorData &other) const
{
    return dateTime == other.dateTime;
}

/*!
 * \brief Get current temperature
 */
double SensorData::getTemperature() const
{
    return temperature;
}

/*!
 * \brief Set current temperature using \a value parameter
 */
void SensorData::setTemperature(double value)
{
    temperature = value;
}

/*!
 * \brief Get current humidity
 */
double SensorData::getHumidity() const
{
    return humidity;
}

/*!
 * \brief Set current humidity using \a value parameter
 */
void SensorData::setHumidity(double value)
{
    humidity = value;
}

/*!
 * \brief Get current pressure
 */
double SensorData::getPressure() const
{
    return pressure;
}

/*!
 * Set current pressure using \a value parameter
 */
void SensorData::setPressure(double value)
{
    pressure = value;
}

/*!
 * \brief Get measurement date
 */
QDateTime SensorData::getDateTime() const
{
    return dateTime;
}

/*!
 * \brief Set measurement date using \a value parameter
 */
void SensorData::setDateTime(const QDateTime &value)
{
    dateTime = value;
}

/*!
 * \brief Calculate cloudiness and returns it
 * TODO implement method
 */
double SensorData::calcCloudiness() const
{
    return 57.12;
}

/*!
 * \brief Calculate perseived temperature and returns it
 * TODO implement method
 */
double SensorData::calcPerseivedTemperature() const
{
    return 23.91;
}

/*!
 * \brief Calculate biometeorological conditions and returns it
 * TODO implement method
 */
bool SensorData::calcBioConds() const
{
    return true;
}

/*!
 * \brief Calculate lunar phase and increasing/decreasing rate and returns them
 * TODO implement method
 */
QPair<double, bool> SensorData::calcLunarProperties() const
{
    return QPair<double, bool>(31.9, false);
}

/*!
 * \brief Serialize sensor readout as JSON object. Output JSON is \a output
 */
void SensorData::serialize(QJsonObject &output) const
{
    output["date"] = dateTime.date().toString(DateFormat);
    output["time"] = dateTime.time().toString(TimeFormat);
    output["temperature"] = temperature;
    output["humidity"] = humidity;
    output["pressure"] = pressure;
}

/*!
 * \brief Deserialize sensor readout from JSON object. Input JSON is \a input
 */
void SensorData::deserialize(const QJsonObject &input)
{
    temperature = input["temperature"].toDouble();
    humidity = input["humidity"].toDouble();
    pressure = input["pressure"].toDouble();
    QDate date = QDate::fromString(input["date"].toString(), DateFormat);
    QTime time = QTime::fromString(input["time"].toString(), TimeFormat);
    dateTime.setDate(date);
    dateTime.setTime(time);
}

/*!
 * \brief Create SensorData object using serialized JSON data. Input JSON is \a input
 */
SensorData SensorData::fromJson(const QJsonObject &input)
{
    SensorData res;
    res.deserialize(input);
    return res;
}

/*!
 * \brief Return string containing all sensor data. This is for testing purposes
 */
QString SensorData::toString() const
{
    QString res("%1 - Temperature: %2 °C, Humidity: %3 %, Pressure %4 hPa");
    return res.arg(dateTime.toString(DateFormat + " " + TimeFormat)).arg(temperature).arg(humidity).arg(pressure);
}
