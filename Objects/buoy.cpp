#include "buoy.h"
#include "streamer.h" //костыль походу

Buoy::Buoy(QString Name, FixedItem *towingPoint, float angleToWired, float wireLength, float AnthenaHeight, float towingDepth) :
    TowedItem(Name, towingPoint, angleToWired, wireLength), AnthenaHeight(AnthenaHeight), towingDepth(towingDepth)
{
    this->itemType = "Buoy";
    if (towingPoint->itemType == "Streamer") {
        dynamic_cast<Streamer*>(towingPoint)->setEndBuoy(this);
    }
}

Buoy::Buoy() {}
