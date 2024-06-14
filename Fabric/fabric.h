#ifndef FABRIC_H
#define FABRIC_H

#include <QString>

#include "fixeditem.h"
#include "toweditem.h"
#include "streamer.h"
#include "connection_creator.h"


class Fabric
{


public:
    struct SchemeCoordinates;
    struct FixedItemInfo;
    struct TowedItemInfo;
    struct StreamerInfo;

    Fabric();

    FixedItem* CreateFixedItem(FixedItemInfo info);
    TowedItem* CreateTowedItem(TowedItemInfo info);

    FixedItem* CreateItem(FixedItemInfo info, bool needConnect);
    TowedItem* CreateItem(TowedItemInfo info, bool needConnect);
    Streamer* CreateItem(StreamerInfo info);




    Connection* createConnection();

    int bindItemConnection(FixedItem* to, Connection* who);

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

    struct StreamerInfo{
        TowedItemInfo towedInfo;
        uint numOfChannels;
        float dCh; //distance between channels
    };

private:
    Connection_creator connectionCreator;

};

#endif // FABRIC_H
