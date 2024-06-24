#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include <QUdpSocket>
#include <QTimer>
#include <QNetworkDatagram>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QSerialPort>
#include <QMessageBox>
#include <QException>
#include <QList>

#include "nmeaparser.h"


namespace Ui {
class Connection;
}

class Connection : public QWidget
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = nullptr);
    virtual ~Connection();

    void recieve_data(QByteArray data); //method for data acquisistion
    void create_file_for_nmea(QString filename); //creating file for NMEA saving



    void write_nmea_data(QByteArray nmea_data);

    ///Quality of data trasmition
    int check_nmea_data(QByteArray nmea_data);
    int check_sum_nmea(QByteArray nmea_data);

    int check_double_package(QByteArray nmea_data);

    //счетчик качества
    uint calcWindow = 20; // размер окна для подсчета качества
    float border = 0.85;
    QList<bool> packetsRecieved;
    QList<bool>::iterator iter;
    float currentQuality = 0;
    float calcQuality(bool recieved);
    //счетчик качества

    ///Quality of data trasmition


    QString filename;
    QFile* file = nullptr;
    QTextStream* datastream = nullptr;

    QString lastRecievedGGA = "GGA empty";
    QString lastRecievedRMC = "RMC empty";

private:

    Ui::Connection *ui;
    NmeaParser nmeaParser;

private slots:
    virtual void ReadyRead(); //slot for getting signal
    // void readPendingDatagrams(); //to TCP
};

#endif // CONNECTION_H
