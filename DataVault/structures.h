#include <QString>

#include "fixeditem.h"
#include "streamer.h"
#include "toweditem.h"
#include "buoy.h"

#ifndef STRUCTURES_H
#define STRUCTURES_H

struct SchemeCoordinates{
    double x;
    double y;
    double z;
    SchemeCoordinates(double x, double y, double z) :
        x(x), y(y), z(z) {};
};

struct FixedItemInfo{
    SchemeCoordinates SchemeCoors;
    QString ItemName;
    FixedItemInfo(double x, double y, double z, QString name) :
        SchemeCoors(x,y,z), ItemName(name) {};
};

struct TowedItemInfo{
    FixedItem* toWhoIsWired;
    double wireLength;
    double angleToWired;
    QString name;
    TowedItemInfo(FixedItem* twiw, double wireLength,
                  double angleToWired, QString Name) :
        toWhoIsWired(twiw), wireLength(wireLength),
        angleToWired(angleToWired), name(Name){};
};

struct StreamerInfo{
    TowedItemInfo towedInfo;
    uint numOfChannels;
    QVector<double> chans; //distance between channels
    StreamerInfo(TowedItemInfo towedInfo,
        uint numOfChannels, QVector<double> chans)
        : towedInfo(towedInfo), numOfChannels(numOfChannels), chans(chans){}
};

struct BuoyInfo{
    TowedItemInfo towedInfo;
    double AnthenaHeight;
    double towingDepth;
    BuoyInfo(TowedItemInfo info, double AnthenaHeight, double towingDepth) :
        towedInfo(info), AnthenaHeight(AnthenaHeight), towingDepth(towingDepth){};
};

struct SourceInfo{
    TowedItemInfo info;
    SourceInfo(TowedItemInfo info) : info(info){};
};



#endif
