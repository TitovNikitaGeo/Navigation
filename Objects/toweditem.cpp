#include "toweditem.h"
#include "buoy.h"
#include "streamer.h"



TowedItem::TowedItem() {}

TowedItem::TowedItem(QString Name,
                     FixedItem* towingPoint, double angle = 0, double wireLength = 5)
    : towingPoint(towingPoint),
    angle(angle), wireLength(wireLength)
{



    this->name = Name;

    this->towingPoint = towingPoint;
    if (QString(towingPoint->metaObject()->className()) == "Streamer") {
        wireLength += dynamic_cast<Streamer*>(towingPoint)->getTotalLength();
    }
    itemType = "Towed";
    double angleInRadians = 90 * M_PI/180;
    x = towingPoint->x * 1 + wireLength*sin(angleInRadians);
    y = towingPoint->y * -1 + wireLength*cos(angleInRadians);
    z = towingPoint->z;
    qDebug() <<QString(this->metaObject()->className())<< " Item Created "<<x<<y<<z<<name;

    qDebug() << name << " towed by "<<towingPoint->name;
    towingPoint->vectorOfConnected.append(this);
    qDebug() << "TowedItem::TowedItem" << towingPoint->name << name;
}

TowedItem::~TowedItem() {
    qDebug() << itemType << " Item Deleted " <<x<<y<<z<<name << "~TowedItem";
    if (hasConnection && connection != nullptr) delete this->connection;
}

void TowedItem::calcIFNotConnected()
{
    NmeaParser parser;
    double azRad = -1;
    if (QString(metaObject()->className()) == "Streamer") {
        Streamer* strm = dynamic_cast<Streamer*>(this);
        if (strm->endBuoy != nullptr) {
            strm->realAzimuthOfTowingRadians = M_PI - atan2(strm->towingPoint->x_coor - strm->endBuoy->x_coor,
                        strm->towingPoint->y_coor - strm->endBuoy->y_coor);

            QGeoCoordinate towingGeoPos = parser.UTMtoGeo(QPointF(strm->towingPoint->x_coor,strm->towingPoint->y_coor));
            QGeoCoordinate endBuoyGeoPos = parser.UTMtoGeo(QPointF(strm->endBuoy->x_coor,strm->endBuoy->y_coor));

            // qDebug() << towingGeoPos << endBuoyGeoPos;
            // qDebug() << qDegreesToRadians(towingGeoPos.azimuthTo(endBuoyGeoPos)) << strm->realAzimuthOfTowingRadians <<__FUNCTION__;
            strm->realAzimuthOfTowingRadians = qDegreesToRadians(towingGeoPos.azimuthTo(endBuoyGeoPos));
            double dx = this->wireLength * qSin(strm->realAzimuthOfTowingRadians);
            double dy = this->wireLength * qCos(strm->realAzimuthOfTowingRadians);

            x_coor = towingPoint->x_coor + dx;
            y_coor = towingPoint->y_coor + dy;


            if (strm->realAzimuthOfTowingRadians < 0) {
                strm->realAzimuthOfTowingRadians += 2*M_PI;
            } else if (strm->realAzimuthOfTowingRadians >= 2 * M_PI) {
                strm->realAzimuthOfTowingRadians -= 2 * M_PI;
            }

            realAzimuthOfTowingRadians = strm->realAzimuthOfTowingRadians;
            azRad = strm->realAzimuthOfTowingRadians;
            strm->calcChansCoors();
        }
        return;
    }

    if (towingPoint->itemType == "Buoy") {
        Buoy* tmp = dynamic_cast<Buoy*>(towingPoint);
        if (tmp-> hasConnection) {
            height = tmp->height - tmp->AnthenaHeight - tmp->towingDepth;
        } else {
            height = tmp->height - tmp->towingDepth;
        }

    } else if(QString(towingPoint->metaObject()->className()) == "FixedItem") {
        height = towingPoint->height - boardHeight; //высота борта больше не захардкожена
    } else {
        height = towingPoint->height;
    }


    x_coor = towingPoint->x_coor + (y-towingPoint->y)*qCos(realAzimuthOfTowingRadians) +
             (x-towingPoint->x)*qSin(realAzimuthOfTowingRadians);
    y_coor = towingPoint->y_coor - (y-towingPoint->y)*qSin(realAzimuthOfTowingRadians) +
             (x-towingPoint->x)*qCos(realAzimuthOfTowingRadians);

    // qDebug() <<x_coor;
    double dx = this->wireLength * qSin(realAzimuthOfTowingRadians);
    double dy = this->wireLength * qCos(realAzimuthOfTowingRadians);
    x_coor = towingPoint->x_coor + dx;
    y_coor = towingPoint->y_coor + dy;


    if (QString(metaObject()->className()) == "Streamer") {
        Streamer* strm = dynamic_cast<Streamer*>(this);
        // qDebug()
        strm->realAzimuthOfTowingRadians = M_PI + atan2(x_coor - strm->endBuoy->x_coor,
                                                        y_coor - strm->endBuoy->y_coor);
        realAzimuthOfTowingRadians = strm->realAzimuthOfTowingRadians;
    }

    // qDebug() << x_coor << y_coor << "AFTER";
    // exit(666);

    QGeoCoordinate tmp = parser.UTMtoGeo(QPointF(x_coor, y_coor));
    this->latitude = tmp.latitude();
    this->longitude = tmp.longitude();
    if (QString(metaObject()->className()) == "Streamer") {
        dynamic_cast<Streamer*>(this)->calcChansCoors();
    }
}


void TowedItem::printSelfInfo() {
    qDebug() <<"printSelfInfo "<<itemType<<x<<y<<z<<name;
}

QString TowedItem::getLastGGA()
{
    QString newGGA = "empty GGA";
    if (connection != nullptr && hasConnection) {
        newGGA = connection->lastRecievedGGA;
        lastGGAData = parser.parseNmeaGGA(newGGA);
        // connection->lastRecievedGGA = "empty GGA";
        // parser.printNmeaGGAData(lastGGAData);
    } else if(towingPoint != nullptr) {
        newGGA = towingPoint->getLastGGA();
        // lastGGAData = towingPoint->lastGGAData;
        // connection->lastRecievedRMC = "empty RMC";
        lastGGAData = parser.parseNmeaGGA(newGGA);
    }
    return newGGA;
}

QString TowedItem::getLastRMC()
{
    QString newRMC = "empty RMC";
    if (connection != nullptr && hasConnection) {
        newRMC = connection->lastRecievedRMC;
        lastRMCData = parser.parseNmeaRMC(newRMC);
        // parser.printNmeaRMCData(lastRMCData);
    } else if(towingPoint != nullptr) {
        newRMC = towingPoint->getLastRMC();
        lastRMCData = towingPoint->lastRMCData;
    }
    return newRMC;
}

void TowedItem::setBoardHeight(double newBoardHeight)
{
    boardHeight = newBoardHeight;
}

void TowedItem::printPos()
{
    qDebug() <<"-----------------------FixedItem::printPos()";
    qDebug() <<name;
    qDebug() << "Height:" << qSetRealNumberPrecision(3) << height  ;
    qDebug() << "UTM East:" << qSetRealNumberPrecision(10)<< x_coor ;
    qDebug() << "UTM North:" << qSetRealNumberPrecision(10) << y_coor;
    qDebug() << "Geo East:" << longitude;
    qDebug() << "Geo Notrh:" << latitude;
    qDebug() << "Azimuth of movement" << azimuthOfMovement;
    qDebug() << "Real Azimuth Of Towing" << qRadiansToDegrees(realAzimuthOfTowingRadians);
}


