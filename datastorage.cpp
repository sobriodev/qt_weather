#include "datastorage.h"
#include <QJsonArray>
#include <QDebug>

/*!
 * \class DataStorage
 * \inmodule core
 * \brief The DataStorage class stores all readouts data and manages serializing and deserializing them
 */

/*!
 * \enum DataStorage::SaveInterval
 * \brief This enum is placeholder when using DataStorage constructor
 * \value Each5Min Save readouts each 5 minutes
 * \value Each10Min Save readouts each 10 minutes
 * \value Each30Min Save readouts each 30 minutes
 * \value EachHour Save readouts each one hour
 */

/*!
 * \variable DataStorage::DefaultValidMinutes
 * \brief The variable stores default saving minutes
 */

/*!
 * \variable DataStorage::JsonKey
 * \brief The variable stores default JSON key used to serialize storage
 */
const QString DataStorage::JsonKey = "sensor-data";

/*!
 * \brief The DataStorage class constructor. The \a validM parameter is used to set data saving interval (in minutes). Correct interval configurations are:
 * \list
 *     \li \b{validM >= 10 AND validM <= 5} Save data with \b{[currentTime] % [validM]} interval. For example setting \a validM to 20 will cause data saving when 00, 20, 40 minutes are reached
 *     \li \b{validM = 60} Save data when next fully hour is reached
 * \endlist
 */
DataStorage::DataStorage(int validM)
{
    if (validM > 60 || validM < 10 || validM % 10 != 0)
        validMinutes = DefaultValidMinutes;
    else
        validMinutes = validM;

    if (validM == 60)
        fullHour = true;
    else
        fullHour = false;
}

/*!
 * \brief Save data readout using \a readout argument. Only valid readouts are saved
 */
void DataStorage::append(const SensorData &readout)
{
    if (validReadout(readout))
        storage.append(readout);
}

/*!
 * \brief Deserialize Storage from JSON object where \a input stores serialized JSON data
 */
void DataStorage::deserialize(const QJsonObject &input)
{
    QJsonArray data = input[JsonKey].toArray();
    for (int i = 0; i < data.size(); i++) {
        QJsonObject rd = data.at(i).toObject();
        if (!deserializable(rd)) {
            qDebug() << "Omitting invalid JSON object";
            continue;
        }

        storage.append(SensorData::fromJson(rd));
    }
}

/*!
 * \brief Serialize Storage data. The \a output argument is an output JSON object
 */
void DataStorage::serialize(QJsonObject &output) const
{
    QJsonArray data;
    for (auto &mes : storage) {
        QJsonObject rd;
        mes.serialize(rd);
        data.append(rd);
    }
    output[JsonKey] = data;
}

/*!
 * \brief Delete old readouts from storage. The cricital delete date is built from \a days and \a timeSub arguments.
 * For example setting \a days argument to \b{5} and \a timeSub to \b{1 hour, 31 minutes and 5 seconds} deletes all readouts older or equal 5 days, 1 hour, 31 minutes and 5 seconds from now
 */
void DataStorage::deleteOldReadouts(int days, const QTime &timeSub)
{
    QDateTime criticalDateTime = QDateTime::currentDateTime().addDays(-days).addSecs(-QTime(0, 0, 0).secsTo(timeSub));
    for (auto &mes : storage) {
        if (mes.getDateTime() <= criticalDateTime) {
            storage.removeAll(mes);
        }
    }
}

/*!
 * \brief The function checks if the data from sensor passed in \a readout argument can be stored. Only readouts with valid date and time will be saved
 */
bool DataStorage::validReadout(const SensorData &readout) const
{
    QTime rdTime = readout.getDateTime().time();
    bool reqCond = (fullHour) ? rdTime.minute() == 0 : (rdTime.minute() % validMinutes == 0);
    if (reqCond) {
        for (auto &mes : storage) {
            if (!compareReadouts(mes, readout))
                return false;
        }
        return true;
    }
    return false;
}

/*!
 * \brief The function compares two readouts by checking their dates. The readout data arguements are passed in the \a first and \a second arguments.
 * This function returns true if the readouts dates are not equal, false otherwise
 */
bool DataStorage::compareReadouts(const SensorData &first, const SensorData &second) const
{
    QDate fRdDate = first.getDateTime().date();
    QDate sRdDate = second.getDateTime().date();
    QTime fRdTime = first.getDateTime().time();
    QTime sRdTime = second.getDateTime().time();

    return fRdDate != sRdDate || fRdTime.hour() != sRdTime.hour() || fRdTime.minute() != sRdTime.minute();
}

/*!
 * \brief This function checks if input JSON object passed in \a input argument is fully deserializable as a SensorData object
 */
bool DataStorage::deserializable(const QJsonObject &input) const
{
    QStringList basicKeys = {"temperature", "pressure", "humidity"};
    for (auto &key : basicKeys) {
        if (!input.contains(key) || !doubleValid(input[key]))
            return false;
    }

    return input.contains("date")
            && dateValid(input["date"].toString())
            && input.contains("time")
            && timeValid(input["time"].toString());
}

/*!
 * \brief This function returns true if \a value parameter can be converted double, false otherwise
 */
bool DataStorage::doubleValid(const QJsonValue &value) const
{
    double res = value.toDouble(INT32_MIN);
    return static_cast<int>(res) != INT32_MIN;
}

/*!
 * \brief This function returns true if \a date argument can be converted to a QDate object, false otherwise
 */
bool DataStorage::dateValid(const QString &date) const
{
    return QDate::fromString(date, SensorData::DateFormat).isValid();
}

/*!
 * \brief This function return true if \a time argument can be converted to a QTime object, false otherwise
 */
bool DataStorage::timeValid(const QString &time) const
{
    return QTime::fromString(time, SensorData::TimeFormat).isValid();
}
