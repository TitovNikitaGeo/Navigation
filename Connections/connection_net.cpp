#include "connection_net.h"
#include "ui_connection_net.h"

Connection_Net::Connection_Net() : ui(new Ui::Connection_Net)
{
    ui->setupUi(this);
}

Connection_Net::Connection_Net(QString IP, int port, QString filename) :
    filename(filename), IP(IP), port(port)
{

    setAttribute(Qt::WA_DeleteOnClose);

    create_file_for_nmea(this->filename); //создание файла
    this->lastRecievedNMEA = "empty net";


    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Connection_Net::onConnected);
    connect(socket,
            &QTcpSocket::readyRead, this, &Connection_Net::ReadyRead);
    connect(socket, &QTcpSocket::
                    disconnected, this, &Connection_Net::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::
            of(&QAbstractSocket::errorOccurred), this, &Connection_Net::onError);
    ///конектим сигналы сокета о событиях с слотами нашего объекта соединения


    socket->connectToHost(QHostAddress(IP), port);

}

// void Connection_Net::recieve_data(QByteArray data) {
//     write_nmea_data(data);
//     ui->label->setText(data);
// }



Connection_Net::~Connection_Net()
{
    file->close();
    socket->close();
    // delete ui;
    delete socket;
}

QString Connection_Net::getIP_port()
{
    return IP + QString::number(port);
}


void Connection_Net::onConnected()
{
    qDebug() << "Connected to" << socket->peerAddress().toString() << ":" << socket->peerPort();
}

void Connection_Net::ReadyRead()
{
    QByteArray data = socket->readAll();
    ///TODO check that buffer is clear
    if (check_nmea_data(data)) {
        recieve_data(data);
        // qDebug() << "GOOD DATA "<< data;
    } else {
        // qDebug() << "BAD DATA "<< data;
    }
}

void Connection_Net::onDisconnected()
{
    qDebug() << "Disconnected from sender";
}

void Connection_Net::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socket->errorString();
}