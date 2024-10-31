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
    double angleInRadians = 0 * M_PI/180;
    x = towingPoint->x * 1 - wireLength;
    y = towingPoint->y;
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
    if (realAzimuthOfTowingRadians == -1) {
        double fakeAzOfTowing = (sharedCircularBuffer.calculateAzimuth());
        if (fakeAzOfTowing > 2*M_PI) fakeAzOfTowing -=2*M_PI;
        if (fakeAzOfTowing < 0) fakeAzOfTowing +=2*M_PI;

        x_coor = towingPoint->x_coor + (x-towingPoint->x)*qSin(fakeAzOfTowing) + (y-towingPoint->y)*qCos(fakeAzOfTowing);
        y_coor = towingPoint->y_coor + (x-towingPoint->x)*qCos(fakeAzOfTowing) - (y-towingPoint->y)*qSin(fakeAzOfTowing);
        // qDebug() << qRadiansToDegrees(fakeAzOfTowing)<< qRadiansToDegrees(sharedCircularBuffer.calculateAzimuth()) << __LINE__;
        // this->showAzDistToObj(towingPoint);
        azimuthOfMovement = sharedCircularBuffer.calculateAzimuth();
    } else {
        x_coor = towingPoint->x_coor - (y-towingPoint->y)*qSin(realAzimuthOfTowingRadians) +
                 (x-towingPoint->x)*qCos(realAzimuthOfTowingRadians);
        y_coor = towingPoint->y_coor + (y-towingPoint->y)*qCos(realAzimuthOfTowingRadians) +
                 (x-towingPoint->x)*qSin(realAzimuthOfTowingRadians);
        // qDebug() << realAzimuthOfTowingRadians;
        this->showAzDistToObj(towingPoint);
    }

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


