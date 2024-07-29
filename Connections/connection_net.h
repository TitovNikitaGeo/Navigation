#ifndef CONNECTION_NET_H
#define CONNECTION_NET_H

#include "connection.h"
#include <QTcpSocket>

namespace Ui {
class Connection_Net;
}

class Connection_Net : public Connection
{
    Q_OBJECT

public:
    explicit Connection_Net();
    Connection_Net(QString IP, int port, QString filename);

    ~Connection_Net() override;
    // void recieve_data(QByteArray data);


    QString filename;
    QString getIP_port();

    int getPort() const;
    void reconnect() override;
private:
    QByteArray data;
    QTcpSocket* socket;
    QString IP;
    int port;
    Ui::Connection_Net *ui;

private slots:
    void ReadyRead() override;


    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
};

#endif // CONNECTION_NET_H
