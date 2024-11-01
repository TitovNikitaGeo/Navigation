#ifndef OBJECTSVAULT_H
#define OBJECTSVAULT_H

#include "structures.h"

#include <QVector>
#include <QFile>

class ItemsStorage
{
public:
    ItemsStorage();

    void SaveItem(FixedItem* NewItem);
    FixedItem* getItem(QString name);
    void DeleteItem(QString Name);
    void setItemForCalculation(QVector<FixedItem*> ItemsVault);


    QVector<FixedItem*> ItemsVault;
    ~ItemsStorage();

    QMutex mutex;
};

#endif // OBJECTSVAULT_H
