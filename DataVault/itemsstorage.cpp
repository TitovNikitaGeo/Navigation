#include "itemsstorage.h"


ItemsStorage::ItemsStorage() {

    ItemsVault.reserve(20);
    //reservation of dynamic memory for 20 elements
}

ItemsStorage::~ItemsStorage() {
    // for (FixedItem* element: ItemsVault){
    //     delete element;
    // }
}

void ItemsStorage::SaveItem(FixedItem* NewItem){
    ItemsVault.append(NewItem);
    // qDebug() <<"I am in Vault "<<NewItem->itemType<<NewItem->name;

}

void ItemsStorage::DeleteItem(QString Name) {
    for (auto it = ItemsVault.begin(); it != ItemsVault.end(); it++) {
        if((*it)->name == Name) {
            delete *it;
        }
    }
}

void ItemsStorage::setItemForCalculation(QVector<FixedItem *> ItemsVault)
{
    // qDebug() << "Вошёл в функцию:" << Q_FUNC_INFO;
    // QVector<FixedItem *> withWhoWeWork;
    // QMutexLocker locker(&mutex); //возможно тут ломается
    FixedItem* itemForCalculation = nullptr;
    for (auto i: ItemsVault) {
        if (i->itemType == "Fixed") {
            if (i->hasConnection) {
                itemForCalculation = i;
                break;
            }
        }
    }
    for (auto i: ItemsVault) {
        if (i->itemType == "Fixed" && !i->hasConnection) {
            i->setItemForCalculations(itemForCalculation);
        }
    }
}

FixedItem* ItemsStorage::getItem(QString name) {
    for (auto it = ItemsVault.begin(); it != ItemsVault.end(); it++) {
        if((*it)->name == name) {
            return *it;
        }
    }
    qDebug() << "No Such Item";
    return nullptr;
}
