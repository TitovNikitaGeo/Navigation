#include "connection_udp.h"

connection_UDP::connection_UDP() {}

connection_UDP::connection_UDP(QString IP, int port, QString filename)
    : IP(IP), port(port), filename(filename)
{
    create_file_for_nmea(this->filename);
    this->lastRecievedGGA = "empty net";
    socket = new QUdpSocket();
    connect(socket, &QUdpSocket::readyRead, this, &connection_UDP::ReadyRead);
    socket->connectToHost(QHostAddress(IP), port);

    setWindowTitle(IP + " "+ QString::number(port));
}

connection_UDP::~connection_UDP() {
    file->close();
    socket->close();
    delete socket;
}


void connection_UDP::ReadyRead()
{
    QByteArray data = socket->readAll();
    if (check_nmea_data(data)) {
        recieve_data(data);
        calcQuality(true);
        qDebug() << "GOOD DATA "<< data;
    } else {
        calcQuality(false);
        qDebug() << "BAD DATA "<< data;
    }
}
