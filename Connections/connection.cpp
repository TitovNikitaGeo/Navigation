#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Connection)
{
    ui->setupUi(this);
    this->hide();


    packetsRecieved.reserve(calcWindow);
    iter = packetsRecieved.end();
    // setAttribute(Qt::WA_DeleteOnClose); //to all successors
}

Connection::~Connection() {
    delete ui;
    file->close();
}

void Connection::write_nmea_data(QByteArray nmea_data){
    int data_condition;
    // qDebug() << "write_nmea_data" << nmea_data;
    if (file  && datastream) {
        data_condition = check_nmea_data(nmea_data);
        if (data_condition == 1) {
            // calcQuality(true);
            *datastream << nmea_data;
            lastRecievedGGA = QString(nmea_data);
            qDebug() << filename.right(filename.lastIndexOf('/')) <<"Connection::write_nmea_data GGA UPDATED";
        }else if(data_condition == 3){
            lastRecievedRMC = QString(nmea_data);
            qDebug() << "Connection::write_nmea_data RMC UPDATED";
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


    timer = new QTimer(this);
    timer->setInterval(10000); // Устанавливаем интервал в 10 секунд

    // Соединяем сигнал timeout() с нашим слотом onTimeout()
    connect(timer, &QTimer::timeout, this, &Connection::onTimeout);
}



int Connection::check_nmea_data(QByteArray nmea_data) {
    int res = 1;
    if (nmea_data.length() < 70){
        qDebug() << "too short check_nmea_data" << nmea_data;
        return 0; // possible package without all data
    }
    if (nmea_data[0] != '$') {  //WTF data (not our package)
        qDebug() << "NOT $ check_nmea_data";
        return 0;
    }
    if (nmea_data.indexOf("\r\n") == -1) {
        qDebug() << "NOT \r\n check_nmea_data";
        return 0; ///not full package
    }
    if (!check_sum_nmea(nmea_data)) {
        qDebug() << "checksum wrong check_nmea_data";
        return 0; //
    }
    if (nmea_data.length() > 89) return 2; // possible double data
    QByteArray rmc = nmea_data.mid(3, 3);
    if (rmc == "RMC") {
        return 3;
    }

    return res;
}

void Connection::recieve_data(QByteArray data) {
    if (check_double_package(data)){
        QStringList nmeaMessages = QString(data).split("\r\n",Qt::SkipEmptyParts);
        write_nmea_data(nmeaMessages[0].append("\r\n").toUtf8());
        write_nmea_data(nmeaMessages[1].append("\r\n").toUtf8());
        return;
    }
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

    //qDebug() << QChar(translatedByte1)<<QChar(translatedByte2) <<"VS"<<QChar(receivedCS0) << QChar(receivedCS1);
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
            // qDebug() << "DOUBLE DATA!";
            // QMessageBox box;
            // box.warning(nullptr,"DOUBLE DATA", "DOUBLE DATA");
            return 1;
        }
    }
    catch(QException ex) {
        qDebug() << ex.what();
        return 0;
    }
    return 0;
}

float Connection::calcQuality(bool recieved) {

    // Наполнение 20
    if (packetsRecieved.size() < calcWindow) {
        packetsRecieved.push_front(recieved);
        if (recieved) {
            currentQuality += (1.0 / calcWindow);
        } else {
            ;// currentQuality -= (1.0 / calcWindow);
        }
        // qDebug() <<"filling " << recieved <<currentQuality<< packetsRecieved.size();
        if (packetsRecieved.size() == 20) iter = packetsRecieved.begin();
        return currentQuality * packetsRecieved.size() / calcWindow;
    }

    // Наполнение 20

    // Пересчет качества

    if (*iter) {
        // qDebug() << *iter << "rise" << currentQuality;
        currentQuality = currentQuality - (1.0 / calcWindow); // Отнимаем самое старое измерение
    }
    if (recieved) {
        // qDebug() << *iter << "fall" <<currentQuality;
        currentQuality += (1.0 / calcWindow); // Прибавляем новое
    }

    // qDebug() << recieved << currentQuality;
    *iter = recieved;

    if (iter == packetsRecieved.end()) {
        iter = packetsRecieved.begin(); // Переход на начало списка, если достигли конца
    } else {
        iter++;
    }

    showSignalQuality(currentQuality);

    return currentQuality;
}

void Connection::setFilename(const QString &newFilename)
{
    filename = newFilename;
}

void Connection::reconnect()
{

}

void Connection::showSignalQuality(float currentQuality)
{
    if (currentQuality > border && !signalTimerAlert) {
        ui->label_2->setText("GOOD");
        QPalette palette = ui->label_2->palette();
        palette.setColor(QPalette::Window, Qt::green);
        ui->label_2->setAutoFillBackground(true);
        ui->label_2->setPalette(palette);
    } else {
        ui->label_2->setText("BAD");
        QPalette palette = ui->label_2->palette();
        palette.setColor(QPalette::Window, Qt::red);
        ui->label_2->setAutoFillBackground(true);
        ui->label_2->setPalette(palette);
    }
}


void Connection::ReadyRead() {
}

void Connection::onTimeout()
{
    // Если таймер сработал, значит, прошло более 10 секунд
    // с последнего хорошего пакета
    signalTimerAlert = true;
    showSignalQuality(currentQuality);
    reconnect();
}



