#include "coordinator.h"

Coordinator::Coordinator(QObject *parent)
    : QObject{parent}
{}

int Coordinator::calcCoors()
{
    try {
        for (FixedItem* item: Vault->ItemsVault) {
            if (item->hasConnection) {
                qDebug() << item->name << item->hasConnection << "Coordinator::calcCoors()";
                //сначала те, что имеют точное положение
                item->getLastGGA();
                item->getLastRMC();
                item->calcItemCoordinates();
                // qDebug() << item->connection->lastRecievedNMEA << "MainWindow::timeToCollectData()";
            }
        }

        for (FixedItem* item: Vault->ItemsVault) { //я вызываю метод предка
            if (item->hasConnection) continue;
            qDebug() << item->name << item->hasConnection;
            QString className = QString(item->metaObject()->className());
            if (className == "Streamer") {
                dynamic_cast<TowedItem*>(item)->calcItemCoordinates();
                dynamic_cast<Streamer*>(item)->calcChansCoors();
            } else if(className == "FixedItem") {
                item->calcItemCoordinates();
            } else if (className == "TowedItem") {
                dynamic_cast<TowedItem*>(item)->calcItemCoordinates();
            } else { //for buoy
                dynamic_cast<TowedItem*>(item)->calcItemCoordinates();
            }
        }
    } catch (std::exception& e) {
        qDebug() << e.what();
        return 0;
    }
    return 1;
}

int Coordinator::printCoors()
{
    try{
        for (FixedItem* item: Vault->ItemsVault) {
            QString className = QString(item->metaObject()->className());
            if (className == "Streamer") {
                dynamic_cast<Streamer*>(item)->printPos();
            } else {
                item->printPos();
            }
        }
    } catch(std::exception& e) {
        qDebug() << e.what();
        return 0;
    }
    return 1;
}

bool Coordinator::wireFixedItems()
{
    FixedItem* ItemWithConnection = nullptr;
    QString className;
    for (FixedItem* item: Vault->ItemsVault){
        className = QString(item->metaObject()->className());
        if (className == "FixedItem") {
            if (item->hasConnection && item->connection != nullptr) {
                ItemWithConnection = item;
                break;
            }
        }
    }
    if (ItemWithConnection == nullptr) {
        return false;
    } else {
        for (FixedItem* item: Vault->ItemsVault){
            if(item->hasConnection) continue;
            className = QString(item->metaObject()->className());
            if (className == "FixedItem") {
                if (!item->hasConnection) {
                    qDebug() << item->name << " wired to " << ItemWithConnection->name;
                    item->ItemForCalculations = ItemWithConnection;
                }
            }
        }
    }
    return true;
}

void Coordinator::boardDepthChanged(double boardHeight)
{
    for (FixedItem* item: Vault->ItemsVault) {
        if (QString(item->metaObject()->className()) != "Fixed Item" && !item->hasConnection
            && QString(item->metaObject()->className()) != "Streamer") {
            TowedItem* towedItem = dynamic_cast<TowedItem*>(item);
            towedItem->setBoardHeight(boardHeight);
        } else if (QString(item->metaObject()->className()) == "Streamer" &&
                   QString(dynamic_cast<TowedItem*>(item)->towingPoint->metaObject()->className()) == "FixedItem"){
            //если коса буксируется не от буя, а то фиксированного объекта на судне
            // item->height = dynamic_cast<TowedItem*>(item)->towingPoint->height - boardHeight;
            dynamic_cast<Streamer*>(item)->setBoardHeight(boardHeight);
        }
    }
    qDebug() << "Coordinator::boardDepthChanged to" << boardHeight;
}
