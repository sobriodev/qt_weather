#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "sensordata.h"
#include <QObject>

class DataStorage
{
public:
    enum SaveInterval {Each5Min = 5, Each10Min = 10, Each30Min = 30, EachHour = 60};
    static const QString JsonKey;
    static const int DefaultValidMinutes = Each10Min;

    explicit DataStorage(int validM = DefaultValidMinutes);

    void append(const SensorData &readout);
    void deserialize(const QJsonObject &input);
    void serialize(QJsonObject &output) const;
    void deleteOldReadouts(int days, const QTime &timeSub);

private:
    bool validReadout(const SensorData &readout) const;
    bool compareReadouts(const SensorData &first, const SensorData &second) const;

    bool deserializable(const QJsonObject &input) const;
    bool doubleValid(const QJsonValue &value) const;
    bool dateValid(const QString &date) const;
    bool timeValid(const QString &time) const;

private:
    QList<SensorData> storage;
    int validMinutes;
    bool fullHour;
};

#endif // SENSORDATALIST_H
