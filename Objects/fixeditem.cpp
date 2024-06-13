#include "fixeditem.h"


FixedItem::FixedItem() {}

FixedItem::FixedItem(float x,float y,float z, QString name) :
    x(x), y(y), z(z), name(name)
{
    itemType = "Fixed Item";
    qDebug() <<itemType<< " Item Created "<<x<<y<<z<<name;

}

FixedItem::~FixedItem() {
    qDebug() <<itemType<< " Item Deleted " <<x<<y<<z<<name;
    deletingWires();
    if (hasConnection && connection != nullptr) delete connection;
}


void FixedItem::set_x(float x) {
    this->x = x;
}

void FixedItem::set_y(float y) {
    this->y = y;
}

void FixedItem::set_z(float z) {
    this->z = z;
}

void FixedItem::deletingWires() {
    for (FixedItem* item: vectorOfConnected) {
        item->lostWire();
    }
}

void FixedItem::lostWire(){
    qDebug() << "Wire Lost Slot activated";
    delete this;
}

QString FixedItem::getLastNmeaStr()
{
    QString res = "empty";
    // if (connection != nullptr) {
    //     res = connection->lastRecievedNMEA;
    // // // parser.printNmeaData(parser.parseNmeaSentence(res));
    // // // return res;
    // // // QString res = "123";
    //     qDebug() << res;
    // }
    qDebug() << "FIXED GET LAST" << connection->filename;
    return res;
}



void FixedItem::newNmeaArived(QString msg) //
{
    NmeaParser::NmeaData data = parser.parseNmeaSentence(msg);
    parser.printNmeaData(data);
}
