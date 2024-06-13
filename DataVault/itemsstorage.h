#ifndef OBJECTSVAULT_H
#define OBJECTSVAULT_H

#include "fabric.h"
#include <QVector>
#include <QFile>

class ItemsStorage
{
public:
    ItemsStorage();

    void SaveFixedItem(FixedItem* NewItem);
    FixedItem* getItem(QString name);
    void DeleteItem(QString Name);

    QVector<FixedItem*> ItemsVault;
    ~ItemsStorage();
};

#endif // OBJECTSVAULT_H
