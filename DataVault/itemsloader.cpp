#include "itemsloader.h"

ItemsLoader &ItemsLoader::instance()
{
    static ItemsLoader instance;
    return instance;
}

ItemsLoader::ItemsLoader(QObject *parent)
    : QObject{parent}
{}

QVector<FixedItem *> ItemsLoader::readFromJSON(QFile *file)
{
    QVector<FixedItem *> res;
    // Ensure the file is open and ready to be read
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open the file.");
        return res;
    }

    // Read the entire content of the file into a QJsonDocument
    QByteArray fileContent = file->readAll();
    file->close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent);
    if (!jsonDoc.isObject()) {
        qWarning("Invalid JSON format.");
        return res;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray itemsArray = jsonObj["items"].toArray();

    for (const QJsonValue &value : itemsArray) {
        QJsonObject jsonItem = value.toObject();
        QString itemType = jsonItem["type"].toString();

        FixedItem* item = nullptr;

        if (itemType == "Fixed") {
            item = new FixedItem();
            item->x = jsonItem["x"].toDouble();
            item->y = jsonItem["y"].toDouble();
            item->z = jsonItem["z"].toDouble();
        }
        else if (itemType == "Towed") {
            TowedItem* towedItem = new TowedItem();
            towedItem->towingPoint->name = jsonItem["twiw"].toString();
            towedItem->wireLength = jsonItem["wireLength"].toDouble();
            item = towedItem;
        }
        else if (itemType == "Buoy") {
            Buoy* buoyItem = new Buoy();
            buoyItem->towingPoint->name = jsonItem["twiw"].toString();
            buoyItem->wireLength = jsonItem["wireLength"].toDouble();
            buoyItem->AnthenaHeight = jsonItem["buoy"].toObject()["AnthenaHeight"].toDouble();
            buoyItem->towingDepth = jsonItem["buoy"].toObject()["towingDepth"].toDouble();
            item = buoyItem;
        }
        else if (itemType == "Streamer") {
            // Streamer* streamerItem = new Streamer();
            QVector<float> chansVector;
            QJsonArray chansArray = jsonItem["Streamer"].toObject()["Channels"].toArray();
            for (const QJsonValue &chanValue : chansArray) {
                chansVector.append(chanValue.toDouble());
            }
            Streamer* streamerItem = new Streamer();
            streamerItem->towingPoint->name = jsonItem["twiw"].toString();
            streamerItem->wireLength = jsonItem["wireLength"].toDouble();

            // for (const QJsonValue &chanValue : chansArray) {
            //     streamerItem->addChannel(chanValue.toDouble());
            // }
            item = streamerItem;

        }

        if (item != nullptr) {
            item->name = jsonItem["name"].toString();
            item->itemType = itemType;
            item->hasConnection = jsonItem["hasConnection"].toBool();
            if (item->hasConnection) {
                item->connection = jsonToConnection(jsonItem["connection"].toObject());
            }

            res.append(item);
        }
    }
    return res;
}

Connection* ItemsLoader::jsonToConnection(const QJsonObject &obj) {
    Connection* conn = nullptr;

    QString filename = obj["filename"].toString();

    if (obj.contains("IP")) {
        Connection_Net* netConn = new Connection_Net(
            obj["IP"].toString(), obj["port"].toInt(), filename);
        netConn->setFilename(filename);
        conn = netConn;
    }
    else if (obj.contains("COMport")) {
        // Connection_com
        Connection_com* comConn = new Connection_com
            (obj["COMport"].toString(), obj["ByteRate"].toInt(), filename);
        conn = comConn;
    }

    return conn;
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
