#ifndef PPKPARSER_H
#define PPKPARSER_H

#include <QObject>
#include <QTextStream>
#include <QRegExp>
#include <QString>
#include <QRegularExpression>

#include "nmeaparser.h"

class PpkParser : public NmeaParser
{
    Q_OBJECT
public:
    PpkParser(QObject *parent = nullptr);
    static double dmsToDecimal(int degrees, int minutes, double seconds);
    static CoordinateData parseLine(const QString &line);
    static bool compareCoordinates(CoordinateData nmea, CoordinateData ppk);


signals:
};

#endif // PPKPARSER_H
