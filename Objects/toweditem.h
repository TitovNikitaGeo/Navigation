#ifndef TOWEDITEM_H
#define TOWEDITEM_H

#include "fixeditem.h"
#include <QtMath>

class TowedItem : public FixedItem
{
private:
    friend class Fabric;
    TowedItem(QString name, FixedItem* towingPoint,
              float angleToWired, float wireLength);
public:
    TowedItem();
    ~TowedItem();

    void printSelfInfo();

    FixedItem* towingPoint;
    float angle; //Angle to ship Dir
    float wireLength;


    QVector<FixedItem*> vectorOfConnections; // not used for now
    // QString name;
    // QString itemType;



};

#endif // TOWEDITEM_H
