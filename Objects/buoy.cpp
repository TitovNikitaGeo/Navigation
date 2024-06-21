#include "buoy.h"

Buoy::Buoy(QString Name, FixedItem *towingPoint, float angleToWired, float wireLength, float AnthenaHeight, float towingDepth) :
    TowedItem(Name, towingPoint, angleToWired, wireLength), AnthenaHeight(AnthenaHeight), towingDepth(towingDepth)
{
    this->itemType = "Buoy";
}

Buoy::Buoy() {}
