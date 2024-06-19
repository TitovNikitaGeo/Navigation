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
    struct NmeaGGAData {
        QGeoCoordinate coordinate;
        QPointF coorUTM;
        QDateTime dateTime;
        float height;
    };
    struct NmeaRMCData{
        double azimuth;
        double speed;
    };

    explicit NmeaParser(QObject *parent = nullptr);


    NmeaGGAData parseNmeaGGA(const QString &nmeaSentence);
    NmeaRMCData parseNmeaRMC(const QString &nmeaSentence);

    QPointF GeoToUTM(const QGeoCoordinate &coordinate);

    void printNmeaGGAData(NmeaGGAData data);
    void printNmeaRMCData(NmeaRMCData data);

private:
    double convertToDegrees(const QString &nmeaValue, const QString &direction);
signals:
};

#endif // NMEAPARSER_H
