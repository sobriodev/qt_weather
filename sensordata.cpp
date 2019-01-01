#include "sensordata.h"

/*!
 * \class SensorData
 * \inmodule core
 * \brief The SensorData class represents single data measurment
 */

/*!
 * \brief The default SensorData constructor sets all sensor data to zero. Optional \a parent object can be sent
 */
SensorData::SensorData(QObject *parent)
    : QObject(parent), temperature(0), humidity(0), pressure(0) {}

/*!
 * \brief The Sensor data constructor initializes all values using \a temperature, \a humidity and \a pressure arguments. Also \a parent object can be sent
 */
SensorData::SensorData(double temperature, double humidity, double pressure, QObject *parent)
    : QObject(parent), temperature(temperature), humidity(humidity), pressure(pressure) {}


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
 * \brief Return string containing all sensor data. This is for testing purposes
 */
QString SensorData::toString() const
{
    QString res("Temperature: %1 °C, Humidity: %2 %, Pressure %3 hPa");
    return res.arg(temperature).arg(humidity).arg(pressure);
}
