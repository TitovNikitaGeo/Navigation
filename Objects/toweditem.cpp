#include "toweditem.h"
#include "buoy.h"

TowedItem::TowedItem() {}

TowedItem::TowedItem(QString Name,
                     FixedItem* towingPoint, float angle = 0, float wireLength = 5)
    : towingPoint(towingPoint),
    angle(angle), wireLength(wireLength)
{
    this->name = Name;

    this->towingPoint = towingPoint;
    itemType = "Towed";
    float angleInRadians = angle * M_PI/180;
    x = towingPoint->x + wireLength*sin(angleInRadians);
    y = towingPoint->y + wireLength*cos(angleInRadians);
    z = towingPoint->z;
    qDebug() <<itemType<< " Item Created "<<x<<y<<z<<name;

    qDebug() << name << " towed by "<<towingPoint->name;
    towingPoint->vectorOfConnected.append(this);

}

TowedItem::~TowedItem() {
    qDebug() << itemType << " Item Deleted " <<x<<y<<z<<name;
    delete this->connection;
}

void TowedItem::calcItemCoordinates()
{
    if (hasConnection && connection) {
        x_coor = lastGGAData.coorUTM.rx();
        y_coor = lastGGAData.coorUTM.ry();
        height = lastGGAData.height;
    } else if (towingPoint != nullptr){
        azimuthOfMovement = towingPoint->azimuthOfMovement;
        double azRad = qDegreesToRadians(azimuthOfMovement);

        ///ДОБАВИТЬ ВЫСОТУ БОРТА
        if (towingPoint->itemType == "Buoy") {
            Buoy* tmp = dynamic_cast<Buoy*>(towingPoint);
            if (tmp-> hasConnection) {
                height = tmp->height - tmp->AnthenaHeight - tmp->towingDepth;
            } else {
                height = tmp->height - tmp->towingDepth;
            }
        } else if(towingPoint->itemType == "Fixed Item") {
            height = towingPoint->height - 5; //высота борта захардкожена
        } else {
            height = towingPoint->height;
        }
        ///ДОБАВИТЬ ВЫСОТУ БОРТА


        x_coor = towingPoint->x_coor + (y-towingPoint->y)*qCos(azRad) +
                 (x-towingPoint->x)*qSin(azRad);
        y_coor = towingPoint->y_coor - (y-towingPoint->y)*qSin(azRad) +
                 (x-towingPoint->x)*qCos(azRad);
    } else {
        qDebug() << "TowedItem::calcItemCoordinates";
        qDebug() << "Если вы здесь, то буквируемое утройство оторвалось";
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
        // parser.printNmeaGGAData(lastGGAData);
    } else if(towingPoint != nullptr) {
        newGGA = towingPoint->getLastGGA();
        lastGGAData = towingPoint->lastGGAData;
        // lastGGAData = parser.parseNmeaGGA(newGGA);
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


