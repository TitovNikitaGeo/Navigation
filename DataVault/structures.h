#include <QString>

#include "fixeditem.h"
#include "streamer.h"
#include "toweditem.h"
#include "buoy.h"

#ifndef STRUCTURES_H
#define STRUCTURES_H

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
    QVector<float> chans; //distance between channels
};

struct BuoyInfo{
    TowedItemInfo towedInfo;
    float AnthenaHeight;
    float towingDepth;
};


#endif
