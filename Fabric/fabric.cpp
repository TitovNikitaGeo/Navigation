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
        info.name, info.toWhoIsWired, info.angleToWired, info.wireLength);
}

Buoy* Fabric::CreateBuoyItem(BuoyInfo info, bool needConnection)
{
    return CreateItem(info, needConnection);
}

FixedItem* Fabric::CreateItem(FixedItemInfo info, bool needConnect)
{
    // qDebug() <<needConnect;
    FixedItem* item =   CreateFixedItem(info);
    if (needConnect) {
        Connection* con = createConnection();
        if (con != nullptr) {
            bindItemConnection(item, con);
        }
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

    ///streamer params dialog
    StreamerDialog dialog;
    if (dialog.exec() == QDialog::Accepted){

        info.chans = dialog.chans;
        info.numOfChannels = dialog.numOfChannels;
        ///streamer params dialog


        Streamer* streamer = new Streamer(info.towedInfo.name,
            info.towedInfo.toWhoIsWired, info.towedInfo.angleToWired,
            info.towedInfo.wireLength, info.numOfChannels, info.chans);
    return streamer;
    } else{
        return nullptr;
    }

}



Buoy* Fabric::CreateItem(BuoyInfo info, bool needConnection)
{
    BuoyDialog dialog(this->MyVault, needConnection);
    if (dialog.exec() == QDialog::Accepted) {
        info.towingDepth = dialog.TowingDepth;
        info.AnthenaHeight = dialog.AnthenaHeight;
        Buoy* newBuoy = new Buoy(info.towedInfo.name,
            info.towedInfo.toWhoIsWired, info.towedInfo.angleToWired,
            info.towedInfo.wireLength, info.AnthenaHeight, info.towingDepth);
        if (needConnection) {
            Connection* con = createConnection();
            if (bindItemConnection(newBuoy, con)){
                qDebug() << "Fabric::CreateItem";
            }
            dialog.selectedStreamer->endBuoy = newBuoy; ///привязываем буй к косе
        }
        return newBuoy;
    } else {
        return nullptr;
    }
}

Connection* Fabric::createConnection()
{
    Connection* res = nullptr;
    if (connectionCreator->exec() == QDialog::Accepted){
        qDebug() << "Fabric::createConnection() Accepted";
        connectionCreator->close();
        res = connectionCreator->getLastConnection();
        return res;
    } else {
        qDebug() << "Fabric::createConnection() Rejected";
        return nullptr;
    }

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
        who->setFilename(to->name);
        return 1;
    } else{
        return 0;
    }
}

void Fabric::setMyVault(ItemsStorage *MyVault)
{
    this->MyVault = MyVault;
}


