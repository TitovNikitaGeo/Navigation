#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTimer>
#include <QNetworkDatagram>
#include <QDataStream>
#include <QMessageBox>
#include <QException>
#include <QTcpSocket>

#include "itemsstorage.h"
#include "logger.h"

namespace Ui {
class Sender;
}

class Sender : public QObject
{
    Q_OBJECT
public:
    Sender();
    ~Sender();
    ItemsStorage* MyVault;
    Logger* log;

    QStringList createPackage();
    void sendPackage(QStringList);
    bool connectToHost(QHostAddress IP, int port);


    void disconnectFromHost();
private:
    QHostAddress IP = QHostAddress::LocalHost;
    int port = 5555;
    QTcpSocket* socket;
};
#endif // SENDER_H
