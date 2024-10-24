#include "fixeditem.h"

double FixedItem::realAzimuthOfTowingRadians = -1;
CircularBuffer FixedItem::sharedCircularBuffer(3);

FixedItem::FixedItem() {}

FixedItem::FixedItem(double x,double y,double z, QString name) :
    x(x), y(y), z(z), name(name)
{
    itemType = "Fixed";
    qDebug() <<itemType<< " Item Created "<<x<<y<<z<<name;
    // logmsg(itemType+" Item Created");
    CircularBuffer& buffer = FixedItem::sharedCircularBuffer;
}

FixedItem::~FixedItem() {
    // qDebug() <<itemType<< " Item Deleted " <<x<<y<<z<<name << "~FixedItem";
    // logmsg(itemType +" Item Deleted " + name);
    deletingWires();
}

void FixedItem::calcItemCoordinates()
{
    if (hasConnection && connection) {
        calcIfConnected();
    } else {
        if (itemType == "Fixed" && ItemForCalculations == nullptr) {
            qWarning() << __FUNCTION__ << "some weird shit";
            exit(-1);
            return; //сюда мы не должны заходить никогда

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
    if (amIItemForCalculating) {
        sharedCircularBuffer.add(lastGGAData.coordinate);
        // qDebug() << name << "I added to CircBuffer" << lastGGAData.coordinate;
    }

    azimuthOfMovement = sharedCircularBuffer.calculateAzimuth();
    // qDebug() << this->name <<"azimuthOfMovement"<<qRadiansToDegrees(azimuthOfMovement);
}

void FixedItem::calcIFNotConnected()
{
    if (QString(metaObject()->className()) == "FixedItem") {
        azimuthOfMovement = sharedCircularBuffer.calculateAzimuth();
        if (azimuthOfMovement == -1) {
            azimuthOfMovement = this->realAzimuthOfTowingRadians;
        }
    } else {
        azimuthOfMovement = ItemForCalculations->azimuthOfMovement;
    }
        // NmeaParser::NmeaGGAData coor = ItemForCalculations->lastGGAData;
    // double azRad = azimuthOfMovement;
    double azRad = sharedCircularBuffer.calculateAzimuth();
    if (azRad == -1) {
        azRad = realAzimuthOfTowingRadians - M_PI;
    }
    if (azRad > 2*M_PI) azRad -= 2*M_PI;
    // qDebug() << qRadiansToDegrees( azRad) << "азимут для рассчета координат"<<__FUNCTION__;

    // qDebug() << name<<__FUNCTION__;
    // qDebug() << ItemForCalculations->name <<__FUNCTION__;
    // double x_tmp = ItemForCalculations->x_coor;
    // double y_tmp = ItemForCalculations->y_coor;
    x_coor = ItemForCalculations->x_coor + (y-ItemForCalculations->y)*qCos(azRad) +
             (x-ItemForCalculations->x)*qSin(azRad);
    y_coor = ItemForCalculations->y_coor - (y-ItemForCalculations->y)*qSin(azRad) +
             (x-ItemForCalculations->x)*qCos(azRad);

    // this->showAzDistToObj(ItemForCalculations);



    QGeoCoordinate curGeoCoordinate = parser.UTMtoGeo(QPointF(x_coor, y_coor));
    // qDebug() << x_coor << QString::number(y_coor) << name;
    // qDebug() << ItemForCalculations->x_coor << QString::number(ItemForCalculations->y_coor) << ItemForCalculations->name;
    // qDebug() << curGeoCoordinate.azimuthTo(ItemForCalculations->lastGGAData.coordinate) << curGeoCoordinate.distanceTo(ItemForCalculations->lastGGAData.coordinate)<< "dist and az by Geo";
    // qDebug() << (M_PI/2 - qAtan2(dy, dx))* (180.0 / M_PI) << qSqrt(dx * dx + dy * dy) << "dist and az by UTM";
    // qDebug() <<"_______________";
    // qDebug() << x_coor << y_coor <<"FixedItem::calcIFNotConnected()";

    latitude = curGeoCoordinate.latitude();
    longitude = curGeoCoordinate.longitude();
    height = ItemForCalculations->height - (z - ItemForCalculations->z);
}

void FixedItem::printPos()
{
    qDebug() <<"-----------------------FixedItem::printPos()";
    qDebug() <<name;
    qDebug() << "Height:" << qSetRealNumberPrecision(3) << height  ;
    qDebug() << "UTM East:" << qSetRealNumberPrecision(10)<< x_coor ;
    qDebug() << "UTM North:" << qSetRealNumberPrecision(10) << y_coor;
    qDebug() << "Geo East:" << longitude;
    qDebug() << "Geo Notrh:" << latitude;
    qDebug() << "Azimuth of movement" << azimuthOfMovement;
    // logmsg(name + "\nHeight " + QString::number(height) +
    //        "\nUTM East:" + QString::number(x_coor) +
    //        "\nUTM North:" + QString::number(y_coor) +
    //        "\nGeo East:" + QString::number(this->longitude) +
    //        "\nGeo Notrh:" + QString::number(this->latitude) +
    //        "Azimuth of movement" + QString::number(azimuthOfMovement));


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
        // connection->lastRecievedGGA = "empty GGA";
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
        // connection->lastRecievedRMC = "empty RMC";
        // parser.printNmeaRMCData(lastRMCData);
    }
    return newRMC;
}



void FixedItem::newNmeaArived(QString msg) //
{
    // NmeaParser::NmeaGGAData data = parser.parseNmeaGGA(msg);
    // parser.printNmeaGGAData(data);
}

void FixedItem::setItemForCalculations(FixedItem *newItemForCalculations)
{
    ItemForCalculations = newItemForCalculations;
    newItemForCalculations->amIItemForCalculating = true;

}

bool FixedItem::showAzDistToObj(FixedItem *to)
{
    QPointF pOther(to->x_coor, to->y_coor);
    QPointF pMy(x_coor, y_coor);
    double dist = pow(((pMy.x() - pOther.x()) * (pMy.x() - pOther.x()) + ((pMy.y() - pOther.y()) * (pMy.y() - pOther.y()))),0.5);
    // if (dist > 190 && dist < 205) return false;
    double deltaX = pMy.x() - pOther.x();
    double deltaY = pMy.y() - pOther.y();
    double azimuthDegrees = (M_PI + qAtan2(deltaX, deltaY)) * 180 / M_PI;
    if (azimuthDegrees < 0) {
        azimuthDegrees += 360.0;
    }
    qDebug() << "Between " << name << " and " << to->name << dist << "Azimuth = " << azimuthDegrees;
    return true;
}
