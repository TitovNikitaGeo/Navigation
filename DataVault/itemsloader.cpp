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


    QVector<QJsonValue> sortedArrayOfJson(jsonObj.size());
    for (auto i:jsonObj) {
        sortedArrayOfJson.push_back(i);
    }
    for (int i = 0; i < sortedArrayOfJson.size()-1; i++) { //bubble sort by creationPriority
        for (int j = 0; j < sortedArrayOfJson.size() - 1 - i; j++) {
            if (sortedArrayOfJson[j]["creationPriority"].toInt() > sortedArrayOfJson[j+1]["creationPriority"].toInt()) {
                std::swap(sortedArrayOfJson[j], sortedArrayOfJson[j+1]);
            }
        }
    }

    for (auto i = sortedArrayOfJson.begin(); i != sortedArrayOfJson.end(); ++i) { //deleting empty JsonValues
        if (!(*i)["name"].toString().isNull()) {
            // qDebug() << *i;
        } else {
            sortedArrayOfJson.erase(i);
        }
    }

    for (auto i = sortedArrayOfJson.begin(); i != sortedArrayOfJson.end(); ++i) {
        res.append(createItemFromJson(*i));
    }

    // for (auto i: res) {
    //     if (i->itemType == "Fixed") {
    //         qDebug() << i->hasConnection << i->name;
    //         if (i->ItemForCalculations) qDebug() <<  i->ItemForCalculations->name;
    //         qDebug();
    //     }
    // }
    ItemsStorage tmp;
    tmp.setItemForCalculation(res);
    // for (auto i: res) {
    //     qDebug() << i->hasConnection << i->name;
    //     if (i->ItemForCalculations) qDebug() <<  i->ItemForCalculations->name;
    //     qDebug();
    // }



    return res;
}

FixedItem* ItemsLoader::createItemFromJson(QJsonValue jsonObject)
{
    static ItemsStorage vault;

    static bool ifStreamerNeedsEndBuoy = false;
    static Streamer* streamerNeedsEndBuoy;
    static QString endBuoyNameForStreamer;
    Fabric fabric;

    FixedItem* item = nullptr;

    QJsonObject jsonItem = jsonObject.toObject();
    QString itemType = jsonItem["type"].toString();
    QString name = jsonItem["name"].toString();
    int creationPriority = jsonItem["creationPriority"].toInt(-1);
    //общие параметры для всех объектов


    if (itemType == "Fixed") {
        FixedItemInfo info(jsonItem["x"].toDouble(0),
                           jsonItem["y"].toDouble(0),jsonItem["z"].toDouble(0),name);
        // item = new FixedItem(info.SchemeCoors.x, info.SchemeCoors.y,info.SchemeCoors.z,name);
        item = fabric.CreateItem(info, false); //создаем без подключения, чтобы не вызывать ConnectionCreator
        vault.SaveItem(item);
    } else {
        //общие параметры для всех буксируемых объектов
        QJsonObject jsonTowedItem = jsonItem["TowedInfo"].toObject();
        QString towingPointName = jsonTowedItem["Towing point"].toString();
        double wireLength = jsonTowedItem["wireLength"].toDouble();


        if (itemType == "Towed") {
            // TowedItem* towedItem = new TowedItem(name, vault.getItem(towingPointName), NULL, wireLength);
            TowedItemInfo info(vault.getItem(towingPointName), wireLength, 90, name);
            TowedItem* towedItem = fabric.CreateItem(info,false);
            item = towedItem;
        } else if (itemType == "Source") {
            TowedItemInfo info(vault.getItem(towingPointName), wireLength, 90, name);
            Source* sourceItem = fabric.CreateItem(SourceInfo(info), false);
            // Source* sourceItem = new Source(name, vault.getItem(towingPointName), NULL, wireLength);
            item = sourceItem;
        } else if (itemType == "Buoy") {
            QJsonObject jsonBuoyItem = jsonTowedItem["buoy"].toObject();
            TowedItemInfo info(vault.getItem(towingPointName), wireLength, 90, name);
            BuoyInfo infoB(info, jsonBuoyItem["AnthenaHeight"].toDouble(), jsonBuoyItem["towingDepth"].toDouble());
            // Buoy* buoyItem  = fabric.CreateItem(infoB, false);
            Buoy* buoyItem = new Buoy(name, vault.getItem(towingPointName), NULL,
                wireLength, jsonBuoyItem["AnthenaHeight"].toDouble(), jsonBuoyItem["towingDepth"].toDouble());
            item = buoyItem;
            if (ifStreamerNeedsEndBuoy && buoyItem->name == endBuoyNameForStreamer) {
                streamerNeedsEndBuoy->setEndBuoy(buoyItem);
            }
        } else if (itemType == "Streamer") {
            QJsonObject jsonStreamerItem = jsonTowedItem["Streamer"].toObject();
            Streamer* streamerItem = new Streamer(name, vault.getItem(towingPointName),
                NULL, wireLength, jsonStreamerItem["NumChanels"].toInt(0), jsonStreamerItem["Step between Channels"].toString());
            item = streamerItem;
            if (jsonStreamerItem["hasEndBuoy"].toBool()) {
                ifStreamerNeedsEndBuoy = true;
                streamerNeedsEndBuoy = streamerItem;
                endBuoyNameForStreamer = jsonStreamerItem["endBuoyName"].toString();
            }
        }
    }
    if (item != nullptr) {
        //общие параметры для соединения
        vault.SaveItem(item);
        item->name = jsonItem["name"].toString();
        item->itemType = itemType;
        item->hasConnection = jsonItem["hasConnection"].toBool();
        if (item->hasConnection) {
            item->connection = jsonToConnection(jsonItem["connection"].toObject());
        }
    }
    item->creationPriority = creationPriority;
    // qDebug() << item->name << item->itemType << item->creationPriority << __FUNCTION__;
    // qDebug();
    vault.setItemForCalculation(vault.ItemsVault);

    return item;

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
    setCreationPriority(ItemsVector);
    QJsonObject obj;
    QJsonObject jsonFixed; //for (int i = ItemsVector.size() - 1; i >= 0; i--) {
    for (int i = 0; i < ItemsVector.size(); i++) {
        FixedItem* item = ItemsVector.at(i);
        jsonFixed["name"] = item->name;
        // qDebug() << "Parameter written: name =" << item->name;

        jsonFixed["creationPriority"] = item->creationPriority;
        jsonFixed["type"] = item->itemType;
        // qDebug() << "Parameter written: type =" << item->itemType;

        jsonFixed["hasConnection"] =  item->hasConnection;
        // qDebug() << "Parameter written: hasConnection =" << item->hasConnection;


        if (item->hasConnection) {
            jsonFixed["connection"] = connectionToJson(item->connection);
            // qDebug() << "Parameter written: connection (complex object)";
        }

        if (item->itemType == "Fixed") {
            jsonFixed["x"] = item->x;
            // qDebug() << "Parameter written: x =" << item->x;

            jsonFixed["y"] = item->y;
            // qDebug() << "Parameter written: y =" << item->y;

        } else if (item->itemType == "Towed" || item->itemType =="Buoy"
                   || item->itemType == "Streamer"|| item->itemType == "Source") {
            QJsonObject jsonTowed;
            TowedItem* itemTowed = dynamic_cast<TowedItem*>(item);
            jsonTowed["Towing point"] = itemTowed->towingPoint->name;
            // qDebug() << "Parameter written:  =" << itemTowed->towingPoint->name;

            jsonTowed["wireLength"] = itemTowed->wireLength;
            // qDebug() << "Parameter written: wireLength =" << itemTowed->wireLength;

            if (item->itemType == "Buoy") {
                QJsonObject jsonBuoy;
                jsonBuoy["AnthenaHeight"] = dynamic_cast<Buoy*>(item)->AnthenaHeight;
                // qDebug() << "Parameter written: AnthenaHeight =" << dynamic_cast<Buoy*>(item)->AnthenaHeight;

                jsonBuoy["towingDepth"] = dynamic_cast<Buoy*>(item)->towingDepth;
                // qDebug() << "Parameter written: towingDepth =" << dynamic_cast<Buoy*>(item)->towingDepth;

                jsonTowed["buoy"] = jsonBuoy;
                // qDebug() << "Parameter written: buoy (complex object)";
            } else if (item->itemType == "Streamer") {
                QJsonObject jsonStreamer;
                QJsonArray chans;
                uint numChans = dynamic_cast<Streamer*>(item)->getChanCount();

                if (dynamic_cast<Streamer*>(item)->endBuoy != nullptr) {
                    jsonStreamer["hasEndBuoy"] = true;
                    jsonStreamer["endBuoyName"] = dynamic_cast<Streamer*>(item)->endBuoy->name;
                } else {
                    jsonStreamer["hasEndBuoy"] = false;
                }

                jsonStreamer["NumChanels"] = (int)numChans;
                jsonStreamer["Step between Channels"] = dynamic_cast<Streamer*>(item)->channelStep;
                // qDebug() << "Parameter written: NumChanels =" << numChans;
                // qDebug() << "Parameter written: Step between Channels =" << dynamic_cast<Streamer*>(item)->channelStep;

                // for (float distance: dynamic_cast<Streamer*>(item)->getChans()) {
                //     chans.append(distance);
                //     qDebug() << "Parameter written: Channel distance =" << distance;
                // }

                // jsonStreamer["Channels"] = chans;
                // qDebug() << "Parameter written: Channels (array)";

                jsonTowed["Streamer"] = jsonStreamer;
                jsonStreamer = QJsonObject();
                // qDebug() << "Parameter written: Streamer (complex object)";
            }
            jsonFixed["TowedInfo"] = jsonTowed;
            jsonTowed = QJsonObject();


        }
        obj[item->name] = jsonFixed;
        jsonFixed = QJsonObject();
    }

    return obj;
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
    obj["type"] = QString(conn->metaObject()->className());
    return obj;
}

bool ItemsLoader::saveJsonObjectToFile(QJsonObject &jsonObject, QString &filePath) {
    // Convert QJsonObject to QJsonDocument
    QJsonDocument jsonDoc(jsonObject);

    // Open the file in write mode
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open file for writing:" << file.errorString();
        return false;
    }

    // Write the JSON document to the file
    file.write(jsonDoc.toJson());
    file.close(); // Close the file

    return true;
}




void ItemsLoader::setCreationPriority(QVector<FixedItem *> &ItemsVault)
{
    FixedItem* itemForCalculation = nullptr;

    QVector<FixedItem *> firstPriority;
    QVector<FixedItem *> secondPriority;
    QVector<TowedItem *> lastPriority;


    for (auto i: ItemsVault) { //разделение объектов по приоритету создания (см. объяснение хэддер FixedItem)
        qDebug() << i->name << i->creationPriority;
        if (QString(i->metaObject()->className()) == "FixedItem") {
            if (i->hasConnection && i->connection != nullptr) {
                itemForCalculation = i;
                firstPriority.push_back(i);
                i->creationPriority = 0;
            } else {
                secondPriority.push_back(i);
                i->creationPriority = 1;
            }
        } else {
            lastPriority.push_back(dynamic_cast<TowedItem*>(i));
        }
    }

    int timeBreakerFlag = 0;
    while (timeBreakerFlag < lastPriority.size() + 1) {
        for (auto i: lastPriority) {
            if (i->towingPoint->creationPriority == -1) {
                continue;
            } else {
                i->creationPriority = i->towingPoint->creationPriority + 2;
            }
        }
        timeBreakerFlag++;
    }
    // for (auto i: ItemsVault) {
    //     qDebug() << i->name << i->creationPriority;
    // }

}
