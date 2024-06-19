#include "coordinator.h"

Coordinator::Coordinator(QObject *parent)
    : QObject{parent}
{}

int Coordinator::calcCoors()
{
    try {
        for (FixedItem* item: Vault->ItemsVault) {
            if (item == nullptr || item->connection == nullptr) {
                item->hasConnection = false;
            }
            if (item->hasConnection) {
                //сначала те, что имеют точное положение
                item->getLastGGA();
                item->getLastRMC();
                item->calcItemCoordinates();
                // qDebug() << item->connection->lastRecievedNMEA << "MainWindow::timeToCollectData()";
            }
        }

        for (FixedItem* item: Vault->ItemsVault) { //я вызываю метод предка
            TowedItem* tmp = dynamic_cast<TowedItem*>(item);
            QString className = QString(item->metaObject()->className());
            if (!(item->hasConnection)){
                tmp->getLastGGA();
                tmp->getLastRMC();
                tmp->calcItemCoordinates();
            }
            if (className == "Streamer") {
                dynamic_cast<Streamer*>(item)->calcChansCoors();
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
