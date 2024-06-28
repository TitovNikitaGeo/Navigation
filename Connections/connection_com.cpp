#include "connection_com.h"
#include "ui_connection_com.h"

Connection_com::Connection_com() : ui(new Ui::Connection_com)
{
    ui->setupUi(this);
}

Connection_com::Connection_com(QString COM_port, int ByteRate, QString filename)
    : filename(filename), ui(new Ui::Connection_com), COM_port(COM_port),
    ByteRate(ByteRate)

{

    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    create_file_for_nmea(this->filename); //создание файла
    // Устанавливаем параметры порта
    SerialPort.setPortName(COM_port);
    SerialPort.setBaudRate(ByteRate);
    // SerialPort.setBaudRate(QSerialPort::Baud115200);

    SerialPort.setDataBits(QSerialPort::Data8);
    SerialPort.setParity(QSerialPort::NoParity);
    SerialPort.setStopBits(QSerialPort::OneStop);
    SerialPort.setFlowControl(QSerialPort::NoFlowControl);

    SerialPort.open(QIODevice::ReadOnly);
    connect(&SerialPort, &QSerialPort::readyRead, this,
            &Connection_com::ReadyRead);
    this->lastRecievedGGA = "empty com";
}

Connection_com::~Connection_com()
{
    // SerialPort.close();
    file->close();
    // delete ui;
}

QString Connection_com::getComPort()
{
    return COM_port;
}

int Connection_com::getByteRate() const
{
    return ByteRate;
}


void Connection_com::ReadyRead() {
    DataBuffer.append(SerialPort.readAll());
    int index;
    if ((index = DataBuffer.indexOf('\n')) != -1) {
        data = DataBuffer.left(index + 1);
        DataBuffer.remove(0, index + 1);
    } else {
        data = DataBuffer + data;
        qDebug() << data << "Data" <<DataBuffer;
        // return;
        if (check_nmea_data(data)) {
            recieve_data(data);
            calcQuality(true);
            qDebug() << "GOOD DATA "<< data;
        } else {
            calcQuality(false);
            qDebug() << "BAD DATA "<< data;
            SerialPort.clear();
        }
        data.clear();
        DataBuffer.clear();
    }
    qDebug() <<"_____________";
}


// void Connection_com::recieve_data(QByteArray data) {
// }
