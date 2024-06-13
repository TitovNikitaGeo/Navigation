#ifndef FABRIC_H
#define FABRIC_H

#include <QString>

#include "fixeditem.h"
#include "toweditem.h"

class Fabric
{


public:
    struct SchemeCoordinates;
    struct FixedItemInfo;
    struct TowedItemInfo;

    Fabric();

    FixedItem* CreateFixedItem(FixedItemInfo info);
    TowedItem* CreateTowedItem(TowedItemInfo info);


    struct SchemeCoordinates{
        float x;
        float y;
        float z;
        SchemeCoordinates(float x, float y, float z) :
            x(x), y(y), z(z) {};
    };

    struct FixedItemInfo{
        SchemeCoordinates SchemeCoors;
        QString ItemName;
        FixedItemInfo(float x, float y, float z, QString name) :
            SchemeCoors(x,y,z), ItemName(name) {};
    };

    struct TowedItemInfo{
        FixedItem* toWhoIsWired;
        float wireLength;
        float angleToWired;
        QString name;
        TowedItemInfo(FixedItem* twiw, float wireLength,
                      float angleToWired, QString Name) :
            toWhoIsWired(twiw), wireLength(wireLength),
            angleToWired(angleToWired), name(Name){};
    };
};

#endif // FABRIC_H
