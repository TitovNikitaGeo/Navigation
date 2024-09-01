#include "sender.h"

Sender::Sender() : socket(new QTcpSocket(this)){
    connectToHost(IP, port);
}

Sender::~Sender()
{
    if (socket->isOpen()) {
        socket->close();
    }
}

QStringList Sender::createPackage()
{
    QStringList res;
    for (FixedItem* item: MyVault->ItemsVault) {
        QString msg;
        msg.append(item->name + "\t" + item->itemType + "\t");
    }
    return res;
}

void Sender::sendPackage(QStringList package)
{
    for (QString i: package) {
        socket->write(i.toStdString().c_str());
    }
}

bool Sender::connectToHost(QHostAddress IP, int port)
{
    socket->connectToHost(IP, port);
    return socket->waitForConnected();
}


void Sender::disconnectFromHost() {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
    }
}
