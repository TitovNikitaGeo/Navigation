#include "fabric.h"

Fabric::Fabric() {
    connectionCreator = new Connection_creator();
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

FixedItem* Fabric::CreateItem(FixedItemInfo info, bool needConnect)
{
    // qDebug() <<needConnect;
    FixedItem* item =   CreateFixedItem(info);
    if (needConnect) {
        Connection* con = createConnection();
        bindItemConnection(item, con);
    }

    return item;
}
TowedItem* Fabric::CreateItem(TowedItemInfo info, bool needConnect)
{
    TowedItem* item = CreateTowedItem(info);
    if (needConnect) {
        Connection* con = createConnection();
        if (bindItemConnection(item, con)){
            qDebug() << "Fabric::CreateItem";
        }
    }
    return item;
}

Streamer* Fabric::CreateItem(StreamerInfo info)
{
    Streamer* streamer = new Streamer(info.towedInfo.name,
        info.towedInfo.toWhoIsWired, info.towedInfo.angleToWired,
        info.towedInfo.wireLength, info.numOfChannels, info.dCh);

    return streamer;
}

Connection* Fabric::createConnection()
{
    Connection* res = nullptr;
    if (connectionCreator->exec() == QDialog::Accepted){
        qDebug() << "Fabric::createConnection()";
    }
    connectionCreator->close();
    res = connectionCreator->getLastConnection();
    return res;
}



int Fabric::bindItemConnection(FixedItem *to, Connection *who)
{
    if (who && to) {
        to->connection = who;
        to->hasConnection = true;
        QString className = who->metaObject()->className();
        // qDebug() << who->metaObject()->className() <<"Class name";
        if (className == QStringLiteral("Connection_com")) {
            Connection_com* tmp = dynamic_cast<Connection_com*>(who);
            qDebug() << tmp->getComPort() <<"Binded to" << to->name;

        } else if(className == QStringLiteral("Connection_Net")) {
            Connection_Net* tmp = dynamic_cast<Connection_Net*>(who);
            qDebug() << tmp->getIP_port()<<"Binded to" << to->name;
        }
        return 1;
    } else{
        return 0;
    }
}


