#include "itemsloader.h"

ItemsLoader::ItemsLoader(QObject *parent)
    : QObject{parent}
{}

QVector<FixedItem *> ItemsLoader::readFromJSON(QFile *file)
{
    QVector<FixedItem *> res;
    return res;
}

QJsonObject ItemsLoader::writeToJSON(QVector<FixedItem *> ItemsVector)
{
    QJsonObject obj;
    QJsonObject jsonFixed;
    for (FixedItem* item: ItemsVector) {
        jsonFixed["name"] = item->name;
        jsonFixed["type"] = item->itemType;
        jsonFixed["hasConnection"] =  item->hasConnection;
        if (item->hasConnection) {
            jsonFixed["connection"] = connectionToJson(item->connection);
        }
        if (item->itemType == "Fixed") {
            jsonFixed["x"] = item->x;
            jsonFixed["y"] = item->y;
            jsonFixed["z"] = item->z;
        } else if (item->itemType == "Towed" || item->itemType =="Buoy" || item->itemType == "Streamer") {
            QJsonObject jsonTowed;
            jsonTowed["twiw"] = dynamic_cast<TowedItem*>(item)->towingPoint->name;
            jsonTowed["wireLength"] = dynamic_cast<TowedItem*>(item)->wireLength;
            if (item->itemType =="Buoy") {
                QJsonObject jsonBuoy;
                jsonBuoy["AnthenaHeight"] = dynamic_cast<Buoy*>(item)->AnthenaHeight;
                jsonBuoy["towingDepth"] = dynamic_cast<Buoy*>(item)->towingDepth;
                jsonTowed["buoy"] = jsonBuoy;
            } else if(item->itemType == "Streamer") {
                QJsonObject jsonStreamer;
                QJsonArray chans;
                uint numChans = dynamic_cast<Streamer*>(item)->getChanCount();
                jsonStreamer["NumChanels"] = (int)numChans;
                for (float distance: dynamic_cast<Streamer*>(item)->getChans()) {
                    chans.append(distance);
                }
                jsonStreamer["Channels"] = chans;
                jsonTowed["Streamer"] = jsonStreamer;
            }
        }
    }
    return obj; //QString(item->metaObject()->className()) == "Towed")
}

QJsonObject ItemsLoader::connectionToJson(Connection *conn) {
    QJsonObject obj;
    obj["filename"] = conn->filename;
    if (QString(conn->metaObject()->className()) == "Connection_Net") {
        obj["IP"] = dynamic_cast<Connection_Net*>(conn)->getIP_port();
        obj["port"] = dynamic_cast<Connection_Net*>(conn)->getPort();
    } else if (QString(conn->metaObject()->className()) == "Connection_com") {
        obj["COMport"] = dynamic_cast<Connection_com*>(conn)->getComPort();
        obj["ByteRate"] = dynamic_cast<Connection_com*>(conn)->getByteRate();
    }
    return obj;
}
