#include "fabric.h"

Fabric::Fabric() {}

FixedItem* Fabric::CreateFixedItem(FixedItemInfo info) {
    FixedItem* NewFixedItem = new FixedItem(
        info.SchemeCoors.x, info.SchemeCoors.y,info.SchemeCoors.z,
        info.ItemName);
    return NewFixedItem;
}

TowedItem* Fabric::CreateTowedItem(TowedItemInfo info) {
    return new TowedItem(
        info.name, info.toWhoIsWired, info.angleToWired, info.wireLength);;
}
