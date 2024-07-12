#include "fixeditem.h"


FixedItem::FixedItem() {}

FixedItem::FixedItem(float x,float y,float z, QString name) :
    x(x), y(y), z(z), name(name)
{
    itemType = "Fixed";
    qDebug() <<itemType<< " Item Created "<<x<<y<<z<<name;

}

FixedItem::~FixedItem() {
    qDebug() <<itemType<< " Item Deleted " <<x<<y<<z<<name << "~FixedItem";
    deletingWires();
}

void FixedItem::calcItemCoordinates()
{
    if (hasConnection && connection) {
        calcIfConnected();
    } else {
        if (ItemForCalculations == nullptr) {
            return;
        } else {
            calcIFNotConnected();
        }
    }
}

void FixedItem::calcIfConnected()
{
    x_coor = lastGGAData.coorUTM.rx();
    y_coor = lastGGAData.coorUTM.ry();
    latitude = lastGGAData.coordinate.latitude();
    longitude = lastGGAData.coordinate.longitude();
    height = lastGGAData.height;
    azimuthOfMovement = lastRMCData.azimuth;
}

void FixedItem::calcIFNotConnected()
{
    azimuthOfMovement = ItemForCalculations->azimuthOfMovement;
    // NmeaParser::NmeaGGAData coor = ItemForCalculations->lastGGAData;
    double azRad = qDegreesToRadians(azimuthOfMovement);
    x_coor = ItemForCalculations->x_coor + (y-ItemForCalculations->y)*qCos(azRad) +
             (x-ItemForCalculations->x)*qSin(azRad);
    y_coor = ItemForCalculations->y_coor - (y-ItemForCalculations->y)*qSin(azRad) +
             (x-ItemForCalculations->x)*qCos(azRad);
    QGeoCoordinate tmp = parser.UTMtoGeo(QPointF(x_coor, y_coor));
    qDebug() << x_coor << y_coor <<"FixedItem::calcIFNotConnected()";
    this->latitude = tmp.latitude();
    this->longitude = tmp.longitude();
    height = ItemForCalculations->height - (z - ItemForCalculations->z);
}

void FixedItem::printPos()
{
    qDebug() <<"-----------------------FixedItem::printPos()";
    qDebug() <<name;
    qDebug() << "Высота:" << qSetRealNumberPrecision(3) << height  ;
    // QPointF utmCoordinates = GeoToUTM(data.coordinate);
    qDebug() << "UTM Восток:" << qSetRealNumberPrecision(10)<< x_coor ;
    qDebug() << "UTM Север:" << qSetRealNumberPrecision(10) << y_coor;
    qDebug() << "Geo восток:" << this->longitude;
    qDebug() << "Geo Север:" << this->latitude;
                                                                    // qDebug() << "Высота над уровнем моря"<< data.height;
    qDebug() << "Азимут движения" << azimuthOfMovement;
}

void FixedItem::deletingWires() {
    for (FixedItem* item: vectorOfConnected) {
        item->lostWire();
    }
}

void FixedItem::lostWire(){
    qDebug() << "Wire Lost Slot activated " << this->name;
    delete this;
}

QString FixedItem::getLastGGA()
{
    QString newGGA = "empty GGA";
    if (connection != nullptr && hasConnection) {
        newGGA = connection->lastRecievedGGA;
        lastGGAData = parser.parseNmeaGGA(newGGA);
        // parser.printNmeaGGAData(lastGGAData);
    }
    return newGGA;
}

QString FixedItem::getLastRMC()
{
    QString newRMC = "empty RMC";
    if (connection != nullptr && hasConnection) {
        newRMC = connection->lastRecievedRMC;
        lastRMCData = parser.parseNmeaRMC(newRMC);
        // parser.printNmeaRMCData(lastRMCData);
    }
    return newRMC;
}



void FixedItem::newNmeaArived(QString msg) //
{
    // NmeaParser::NmeaGGAData data = parser.parseNmeaGGA(msg);
    // parser.printNmeaGGAData(data);
}
