#include "nmeaparser.h"


NmeaParser::NmeaParser(QObject *parent)
    : QObject{parent}
{

}

NmeaParser::NmeaData NmeaParser::parseNmeaSentence(const QString &nmeaSentence) {
    NmeaData data;

    if (nmeaSentence.startsWith("$GPGGA") || nmeaSentence.startsWith("$GNGGA")) {
        QStringList parts = nmeaSentence.split(',');

        if (parts.size() > 9) {
            QString timeValue = parts[1];
            QString latitudeValue = parts[2];
            QString latitudeDirection = parts[3];
            QString longitudeValue = parts[4];
            QString longitudeDirection = parts[5];
            QString heightValue = parts[9];



            double latitude = convertToDegrees(latitudeValue, latitudeDirection);
            double longitude = convertToDegrees(longitudeValue, longitudeDirection);
            float height = heightValue.toFloat();

            int hours = timeValue.mid(0, 2).toInt();
            int minutes = timeValue.mid(2, 2).toInt();
            int seconds = timeValue.mid(4, 2).toInt();
            QTime time(hours, minutes, seconds);
            QDateTime dateTime(QDateTime::currentDateTime().date(), time);



            data.coordinate = QGeoCoordinate(latitude, longitude);

            qDebug()<<latitudeValue<<longitudeValue<<"_____________";
            qDebug()<<data.coordinate.latitude()<<data.coordinate.longitude()<<"_____________";

            QPointF coorUTM= GeoToUTM(data.coordinate);
            data.coorUTM = coorUTM;
            data.dateTime = dateTime;
            data.height = height;
        }
    }

    return data;
}

double NmeaParser::convertToDegrees(const QString &nmeaValue, const QString &direction) {
    if (nmeaValue.isEmpty() || direction.isEmpty()) {
        return 0.0;
    }


    double degrees;
    double minutes;
    double decimalDegrees;
    bool ok;

    if (direction == "S" || direction == "N" ) {
        degrees = nmeaValue.left(2).toDouble(&ok);
        minutes = nmeaValue.mid(2).toDouble(&ok);
        decimalDegrees = degrees + (minutes / 60.0);
        if (!ok) return 0.0;
    } else {
        degrees = nmeaValue.left(3).toDouble(&ok);
        minutes = nmeaValue.mid(3).toDouble(&ok);
        decimalDegrees = degrees + (minutes / 60.0);
        if (!ok) return 0.0;
    }

    if (direction == "S" || direction == "W") {
        decimalDegrees = -decimalDegrees;
    }

    return decimalDegrees;
}

void NmeaParser::printNmeaData(NmeaData data) {
    if (data.coordinate.isValid()) {
        qDebug() << "Широта:" << data.coordinate.latitude();
        qDebug() << "Долгота:" << data.coordinate.longitude();
        qDebug() << "Время:" << data.dateTime.toString();
        qDebug() << "Высота:" << data.height;

        // QPointF utmCoordinates = GeoToUTM(data.coordinate);
        qDebug() << "UTM Восток:" << data.coorUTM.x();
        qDebug() << "UTM Север:" << data.coorUTM.y();
    } else {
        qDebug() << "Некорректная строка NMEA";
    }
}

QPointF NmeaParser::GeoToUTM(const QGeoCoordinate &coordinate) {
    double k0 = 0.9996;
    double a = 6378137.0;
    double eccSquared = 0.00669438;
    double eccPrimeSquared;
    double e1 = (1 - std::sqrt(1 - eccSquared)) / (1 + std::sqrt(1 - eccSquared));
    double N, T, C, A, M;
    double LatRad = coordinate.latitude() * M_PI / 180.0;
    double LongRad = coordinate.longitude() * M_PI / 180.0;
    double LongOriginRad;
    int ZoneNumber = std::floor((coordinate.longitude() + 180) / 6) + 1;
    LongOriginRad = ((ZoneNumber - 1) * 6 - 180 + 3) * M_PI / 180.0;

    eccPrimeSquared = (eccSquared) / (1 - eccSquared);

    N = a / std::sqrt(1 - eccSquared * std::sin(LatRad) * std::sin(LatRad));
    T = std::tan(LatRad) * std::tan(LatRad);
    C = eccPrimeSquared * std::cos(LatRad) * std::cos(LatRad);
    A = std::cos(LatRad) * (LongRad - LongOriginRad);

    M = a * ((1 - eccSquared / 4 - 3 * eccSquared * eccSquared / 64 - 5 * eccSquared * eccSquared * eccSquared / 256) * LatRad
             - (3 * eccSquared / 8 + 3 * eccSquared * eccSquared / 32 + 45 * eccSquared * eccSquared * eccSquared / 1024) * std::sin(2 * LatRad)
             + (15 * eccSquared * eccSquared / 256 + 45 * eccSquared * eccSquared * eccSquared / 1024) * std::sin(4 * LatRad)
             - (35 * eccSquared * eccSquared * eccSquared / 3072) * std::sin(6 * LatRad));

    double UTMEasting = (k0 * N * (A + (1 - T + C) * A * A * A / 6
                                   + (5 - 18 * T + T * T + 72 * C - 58 * eccPrimeSquared) * A * A * A * A * A / 120)
                         + 500000.0);

    double UTMNorthing = (k0 * (M + N * std::tan(LatRad) * (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24
                                                            + (61 - 58 * T + T * T + 600 * C - 330 * eccPrimeSquared) * A * A * A * A * A * A / 720)));

    if (coordinate.latitude() < 0) {
        UTMNorthing += 10000000.0;
    }

    return QPointF(UTMEasting, UTMNorthing);
}








