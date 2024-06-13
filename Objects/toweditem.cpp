#include "toweditem.h"

TowedItem::TowedItem() {}

TowedItem::TowedItem(QString Name,
                     FixedItem* towingPoint, float angle = 0, float wireLength = 5)
    : towingPoint(towingPoint),
    angle(angle), wireLength(wireLength)
{
    this->name = Name;


    itemType = "Towed";
    float angleInRadians = angle * M_PI/180;
    x = towingPoint->x + wireLength*sin(angleInRadians);
    y = towingPoint->y + wireLength*cos(angleInRadians);
    z = towingPoint->z;
    qDebug() <<itemType<< " Item Created "<<x<<y<<z<<name;


    qDebug() << "adding this to "<<towingPoint->name;
    towingPoint->vectorOfConnected.append(this);

    // printSelfInfo();
}

TowedItem::~TowedItem() {
    qDebug() << itemType << " Item Deleted " <<x<<y<<z<<name;
    delete this->connection;
}


void TowedItem::printSelfInfo() {
    qDebug() <<"printSelfInfo "<<itemType<<x<<y<<z<<name;
}


