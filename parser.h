#ifndef PARSER_H
#define PARSER_H

#include <QObject>

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);
    void parseSensorData(const QString &data);

signals:
    void temperatureChanged(double temperature);
    void humidityChanged(double humidity);
    void pressureChanged(double pressure);
};

#endif // PARSER_H
