#ifndef ITEMSLOADER_H
#define ITEMSLOADER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "fabric.h"
#include "itemsstorage.h"

class ItemsLoader : public QObject
{
    Q_OBJECT
    static ItemsLoader& instance();
    explicit ItemsLoader(QObject *parent = nullptr);
public:
    static Connection* jsonToConnection(const QJsonObject &obj);
    static QVector<FixedItem*> readFromJSON(QFile* file);
    static QJsonObject  writeToJSON(QVector<FixedItem*> ItemsVector);
    static QJsonObject connectionToJson(Connection *conn);
    static void saveJsonFile(QJsonDocument doc);
signals:
};

#endif // ITEMSLOADER_H
