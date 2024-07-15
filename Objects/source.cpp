#include "source.h"

Source::Source() {
    this->itemType = "Source";
}

Source::Source(QString Name, FixedItem *towingPoint, float angle, float wireLength)
    :TowedItem(Name, towingPoint, angle, wireLength)
{
    itemType = "Source";
}
