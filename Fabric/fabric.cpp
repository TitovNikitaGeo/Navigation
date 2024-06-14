#include "fabric.h"

Fabric::Fabric() {
}

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

int Fabric::bindItemConnection(FixedItem *to, Connection *who)
{
    if (to->hasConnection && who) {
        to->connection = who;
        if (who->metaObject()->className() == "Connection_com") {
            Connection_com* tmp = dynamic_cast<Connection_com*>(who);
            qDebug() << tmp->getComPort() <<"Binded to" << to->name;
        } else if(who->metaObject()->className() == "Connection_Net") {
            Connection_Net* tmp = dynamic_cast<Connection_Net*>(who);
            qDebug() << tmp->getIP_port()<<"Binded to" << to->name;
        }
        return 1;
    } else{
        return 0;
    }
}


