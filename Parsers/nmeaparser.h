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
    enum NmeaType{
        GGA,
        RMC
    };

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

    QTime getTimeFromNmeaGGA(const QString &nmeaSentence);
    NmeaGGAData parseNmeaGGA(const QString &nmeaSentence);
    NmeaRMCData parseNmeaRMC(const QString &nmeaSentence);

    QPointF GeoToUTM(const QGeoCoordinate &coordinate);

    void printNmeaGGAData(NmeaGGAData data);
    void printNmeaRMCData(NmeaRMCData data);

    QGeoCoordinate UTMtoGeo(const QPointF &coordinate);

    static bool isValid(NmeaGGAData nmeaGGA);
    static bool isValid(NmeaRMCData nmeaRMC);
private:
    double convertToDegrees(const QString &nmeaValue, const QString &direction);

    bool isNorth = true;

    //constants for calculation
    static int ZoneNumber; //сохраняем номер зоны
    const double k0 = 0.9996;
    const double a = 6378137.0;  // WGS84 major axis
    const double eccSquared = 0.00669438;  // WGS84 eccentricity squared
    const double eccPrimeSquared = eccSquared / (1 - eccSquared);
    const double e1 = (1 - sqrt(1 - eccSquared)) / (1 + sqrt(1 - eccSquared));
signals:
};

#endif // NMEAPARSER_H
