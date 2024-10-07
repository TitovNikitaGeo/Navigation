#include "toweditem.h"
#include "buoy.h"
#include "streamer.h"

float TowedItem::realAzimuthOfTowingRadians = -1;

TowedItem::TowedItem() {}

TowedItem::TowedItem(QString Name,
                     FixedItem* towingPoint, float angle = 0, float wireLength = 5)
    : towingPoint(towingPoint),
    angle(angle), wireLength(wireLength)
{
    this->name = Name;

    this->towingPoint = towingPoint;
    if (QString(towingPoint->metaObject()->className()) == "Streamer") {
        wireLength += dynamic_cast<Streamer*>(towingPoint)->getTotalLength();
    }
    itemType = "Towed";
    float angleInRadians = angle * M_PI/180;
    x = towingPoint->x + wireLength*sin(angleInRadians);
    y = towingPoint->y + wireLength*cos(angleInRadians);
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
    double azRad = -1;
    if (towingPoint != nullptr) {
        if (QString(metaObject()->className()) == "Streamer") {
            Streamer* strm = dynamic_cast<Streamer*>(this);
            if (strm->endBuoy != nullptr) {
                strm->realAzimuthOfTowingRadians = atan2(strm->towingPoint->x_coor - strm->endBuoy->x_coor,
                                                    strm->towingPoint->y_coor - strm->endBuoy->y_coor);
                if (strm->realAzimuthOfTowingRadians < 0) {
                    strm->realAzimuthOfTowingRadians += 2*M_PI;
                }
                // strm->realAzimuthOfTowingRadians =
                //     ((strm->towingPoint->x_coor - strm->endBuoy->x_coor)/
                //     (strm->towingPoint->y_coor - strm->endBuoy->y_coor));
                // qDebug() << qRadiansToDegrees( strm->realAzimuthOfTowingRadians )<< __FUNCTION__;
                azRad = strm->realAzimuthOfTowingRadians;
                // qDebug() <<azRad;
                // qDebug() << strm->towingPoint->x_coor << strm->towingPoint->y_coor << "STP";
                // qDebug() << strm->endBuoy->x_coor << strm->endBuoy->y_coor << "ENDBUOY";
                // qDebug() << __FUNCTION__;

            }
            strm->calcChansCoors();
            // azimuthOfMovement = towingPoint->azimuthOfMovement;
            // azRad = azimuthOfMovement;
        } else {
            azRad = realAzimuthOfTowingRadians;
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
        // qDebug() << x_coor << y_coor << "BEFORE";

        x_coor = towingPoint->x_coor + (y-towingPoint->y)*qCos(azRad) +
                 (x-towingPoint->x)*qSin(azRad);
        y_coor = towingPoint->y_coor - (y-towingPoint->y)*qSin(azRad) +
                 (x-towingPoint->x)*qCos(azRad);

        // qDebug() << x_coor << y_coor << "AFTER";
        // exit(666);

        QGeoCoordinate tmp = parser.UTMtoGeo(QPointF(x_coor, y_coor));
        this->latitude = tmp.latitude();
        this->longitude = tmp.longitude();

    } else {
        qDebug() << __FUNCTION__;
        qDebug() << "Если вы здесь, то буквируемое утройство оторвалось";
        qDebug() << "Или вы где-то ошиблись...";
        qDebug() << "...Например, в выборе софта";
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

void TowedItem::setBoardHeight(float newBoardHeight)
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


