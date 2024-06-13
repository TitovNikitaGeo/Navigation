#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Connection)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose); //to all successors
}

Connection::~Connection() {
    delete ui;
}

// Connection::~Connection()
// {
//     qDebug() << file->size() << "Connection killed";
//     // file->close();
//     // delete file;
//     // delete socket;
//     // delete datastream;
//     // delete ui;
// }

// void Connection::readPendingDatagrams() //to TCP
// {
//     QNetworkDatagram datagram = socket->receiveDatagram();
//     QByteArray message = datagram.data();
//     ui->label->setText(message);
//     // qDebug() << message << filename;
//     write_nmea_data(message);
//     // QStringList list = message.split(',');
// }

void Connection::write_nmea_data(QByteArray nmea_data){
    int data_condition;
    if (file  && datastream) {
        data_condition = check_nmea_data(nmea_data);
        if (data_condition == 2) { //possible double package
            // qDebug() << nmea_data <<"DOUBLE DATA In write_nmea_data";
            QStringList nmeaMessages = QString(nmea_data).split("\r\n",Qt::SkipEmptyParts);
            write_nmea_data(nmeaMessages[0].append("\r\n").toUtf8()); //спросить, стоит ли так делать у Артема
            write_nmea_data(nmeaMessages[1].append("\r\n").toUtf8()); //грязные мерзкие приемы
            //
            return;
        } else if (data_condition == 1) {
            *datastream << nmea_data;
            lastRecievedNMEA = QString(nmea_data);
            // return;
            // nmeaParser.printNmeaData(nmeaParser.parseNmeaSentence(QString(nmea_data)));
            qDebug() << nmea_data << " to file" << filename;
        } else {
            qDebug() << "BAD PACKAGE" << nmea_data;
            return;
        }
    } else {
        qDebug() << "File/Datastream not created";
    }
}

void Connection::create_file_for_nmea(QString filename){
    if (!file) {
        file = new QFile(filename);
    } else {
        if (file->isOpen()) file->close();
    }

    file->open(QIODevice::Append);
    if (!file->isOpen()) {
        QMessageBox box;
        box.critical(nullptr, "Error", "File not created");
        return;
    }
    qDebug() << "File " << filename << "opened in A mode";

    datastream = new QTextStream (file);
    this->filename = filename;
}

int Connection::check_nmea_data(QByteArray nmea_data) {
    int res = 1;
    if (nmea_data[0] != '$') return 0; //WTF data (not our package)
    if (nmea_data.indexOf("\r\n") == -1) return 0; ///not full package
    if (nmea_data.length() < 70) return 0; // possible package without all data
    if (nmea_data.length() > 87) return 2; // possible double data
    if (!check_sum_nmea(nmea_data)) return 0; //
    /// TODO: check data format /// must be time and coordinates
    return res;
}

void Connection::recieve_data(QByteArray data) {
    write_nmea_data(data);
    ui->label->setText(data);
}



int Connection::check_sum_nmea(QByteArray nmea_data)
{
    uint8_t calcChecksum = 0;
    int packetEndIndex = nmea_data.indexOf('*'); // Индекс символа *, перед которым идет контрольная сумма

    for(int i=1; i<packetEndIndex; ++i) // Подсчет контрольной суммы от символа после $ до *
    {
        calcChecksum = calcChecksum ^ nmea_data[i]; //XOR
    }

    uint8_t nibble1 = (calcChecksum & 0xF0) >> 4;
    uint8_t nibble2 = calcChecksum & 0x0F;



    uint8_t translatedByte1 = (nibble1 <= 0x9) ? (nibble1 + '0') : (nibble1 - 10 + 'A');
    uint8_t translatedByte2 = (nibble2 <= 0x9) ? (nibble2 + '0') : (nibble2 - 10 + 'A');

    // QByteArray receivedCS = nmea_data.mid(nmea_data.length()-2);
    uint8_t receivedCS0 = nmea_data[nmea_data.length()-4];
    uint8_t receivedCS1 = nmea_data[nmea_data.length()-3];
    // Получение контрольной суммы из пакета

    if(translatedByte1 == receivedCS0 && translatedByte2 == receivedCS1)
    // Сравнение контрольной суммы с полученной из пакета
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Connection::check_double_package(QByteArray nmea_data) {
    try{
        QStringList nmeaMessages = QString(nmea_data).split("\r\n",Qt::SkipEmptyParts);
        if (nmeaMessages.size() == 2) {
            qDebug() << "DOUBLE DATA!";
            QMessageBox box;
            box.warning(nullptr,"DOUBLE DATA", "DOUBLE DATA");
            return 1;
        }
    }
    catch(QException ex) {
        qDebug() << ex.what();
        return 0;
    }
    return 0;
}


void Connection::ReadyRead() {
}



