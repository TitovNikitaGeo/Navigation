#ifndef BUOY_H
#define BUOY_H

#include "toweditem.h"

class Buoy : public TowedItem
{
    Q_OBJECT
    friend class Fabric;
    Buoy(QString Name,
         FixedItem* towingPoint, float angle,
         float wireLength,float AnthenaHeight, float towingDepth);


public:
    Buoy();


    float AnthenaHeight = 1;
    float towingDepth = 1;
};

#endif // BUOY_H
