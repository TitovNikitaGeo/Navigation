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
    explicit ItemsLoader(QObject *parent = nullptr);
public:
    static ItemsLoader& instance();
    static Connection* jsonToConnection(const QJsonObject &obj);
    static QVector<FixedItem*> readFromJSON(QFile* file);
    static QJsonObject  writeToJSON(QVector<FixedItem*> ItemsVector);
    static QJsonObject connectionToJson(Connection *conn);
    static QJsonDocument createJsonDoc(QJsonObject obj);
    static bool saveJsonObjectToFile(QJsonObject &jsonObject, QString &filePath);
    // static bool saveJsonObjectToFile(const QJsonObject &jsonObject, const QString &filePath);
signals:
};

#endif // ITEMSLOADER_H
