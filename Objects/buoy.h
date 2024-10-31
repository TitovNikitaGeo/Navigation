#ifndef BUOY_H
#define BUOY_H

#include "toweditem.h"
// #include "structures.h"

class Buoy : public TowedItem
{
    Q_OBJECT
    friend class Fabric;
    friend class ItemsLoader;
    Buoy(QString Name,
         FixedItem* towingPoint, float angleToWired,
         float wireLength,float AnthenaHeight, float towingDepth);
public:
    Buoy();


    float AnthenaHeight = 1;
    float towingDepth = 1;
};

#endif // BUOY_H
