#ifndef NMEAPARSER_H
#define NMEAPARSER_H

#include <QObject>
#include <QGeoCoordinate>
#include <QTime>
#include <QDateTime>
#include <QString>
#include <QPointF>


class NmeaParser : public QObject
{
    Q_OBJECT
public:
    struct NmeaData {
        QGeoCoordinate coordinate;
        QPointF coorUTM;
        QDateTime dateTime;
        float height;
    };

    explicit NmeaParser(QObject *parent = nullptr);


    NmeaData parseNmeaSentence(const QString &nmeaSentence);

    QPointF GeoToUTM(const QGeoCoordinate &coordinate);
    void printNmeaData(NmeaData data);

private:
    double convertToDegrees(const QString &nmeaValue, const QString &direction);
signals:
};

#endif // NMEAPARSER_H
