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

    static QVector<FixedItem*> readFromJSON(QFile* file);
    static QJsonObject  writeToJSON(QVector<FixedItem*> ItemsVector);
    static QJsonObject connectionToJson(Connection *conn);
signals:
};

#endif // ITEMSLOADER_H
