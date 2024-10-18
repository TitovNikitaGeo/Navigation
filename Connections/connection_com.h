#ifndef CONNECTION_COM_H
#define CONNECTION_COM_H

#include "connection.h"

#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class Connection_com;
}

class Connection_com : public Connection
{
    Q_OBJECT

public:
    explicit Connection_com();
    Connection_com(QString COM_port, int ByteRate, QString filename);

    ~Connection_com() override;
    // void recieve_data(QByteArray data);

    QString filename;
    QString getComPort();

    int getByteRate() const;
    void reconnect() override;

private:
    Ui::Connection_com *ui;
    QSerialPort SerialPort;

    QByteArray data;
    QString COM_port;
    int ByteRate = 19200;


private slots:
    void ReadyRead() override;
};

#endif // CONNECTION_COM_H
