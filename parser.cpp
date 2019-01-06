#include "parser.h"
#include <QDebug>
#include <QChar>

/*!
 * \class Parser
 * \inmodule core
 * \brief The Parser class provides a way to parse crude sensor data
 */

/*!
 * \variable Parser::SensorsCount
 * \brief The variable stores number of sensors used
 */

/*!
 * \brief The parser class constructor with optional \a parent object
 */
Parser::Parser(QObject *parent) : QObject(parent) {}

/*!
 * \fn void Parser::temperatureChanged(double temperature)
 * \brief The signal is emmited when temperature has changed. The \a temperature parameter holds the actual value
 */

/*!
 * \fn void Parser::humidityChanged(double humidity)
 * \brief The signal is emmited when humidity has changed. The \a humidity parameter holds the actual value
 */

/*!
 * \fn void Parser::pressureChanged(double pressure)
 * \brief The signal is emmited when pressure has changed. The \a pressure parameter holds the actual value
 */

/*!
 * \fn void allDataChanged(SensorData data)
 * \brief The signal is emmited when all data has changed. The \a data pressure parameter holds the actual object
 */

/*!
 * \brief Parse crude sensor data. The function will fill in \a data if all data is gained. The \a data format is \b {T{value}|H{value}|P{value}}, where \b {{value}} is a placeholder for real sensor value.
 * This function emits
 * \list
 *     \li \l Parser::temperatureChanged()
 *     \li \l Parser::humidityChanged()
 *     \li \l Parser::pressureChanged()
 *     \li \l Parser::allDataChanged()
 * \endlist
 */
void Parser::parseSensorData(const QString &data)
{
    QStringList values = data.split("|");
    SensorData res;

    for (QString &value : values) {
        char type = value.at(0).toLatin1();
        double readout = value.remove(0, 1).toDouble();

        switch (type) {
        case 'T':
            res.setTemperature(readout);
            emit temperatureChanged(readout);
            break;
        case 'P':
            res.setPressure(readout);
            emit pressureChanged(readout);
            break;
        case 'H':
            res.setHumidity(readout);
            emit humidityChanged(readout);
            break;
        default:
            break;
        }
    }

    if (values.count() == SensorsCount) {
        res.setDateTime(QDateTime::currentDateTime());
        emit allDataChanged(res);
    }
}
