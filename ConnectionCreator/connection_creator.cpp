#include "connection_creator.h"
#include "ui_connection_creator.h"



Connection_creator::Connection_creator(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Connection_creator)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &port : ports) {
        ui->PortsAvailableComboBox->addItem(port.portName());
    }

    if (ports.isEmpty() && ui->connection_types->currentIndex() == 1) {
        ui->Error_label_COM_tab->setStyleSheet("QLabel {color : red; }");
        ui->Error_label_COM_tab->setText("No COM ports available");
        ui->pushButton->setEnabled(false);
    }
    ui->BRate_choose->setCurrentIndex(1);
    setWindowTitle("Create new connection");
    // connect(ui->pushButton, &QPushButton::clicked, this, &Connection_creator::on_pushButton_clicked);
}

Connection_creator::~Connection_creator()
{
    qDebug() << "Connection_creator destroyed";

    for (auto connection : Connection_vector) {
        try{
            delete connection;
            delete ui;
        } catch (const std::exception &e) {
            qDebug() << e.what();
        }
    }


}

void Connection_creator::on_pushButton_clicked()
{
    Connection* newConnection = createConnection();
    if (newConnection) {
        Connection_vector.append(newConnection);
        newConnection->show();

    }
    lastConnection = newConnection;
    accept();
    this->close();
}

Connection* Connection_creator::createConnection()
{

    // QString fileName = QFileDialog::getSaveFileName(
    //     nullptr,
    //     "Save NMEA File",
    //     dirPath.absolutePath(),
    //     "NMEA Files (*.nmea);;All Files (*)",
    //     nullptr,
    //     QFileDialog::DontConfirmOverwrite);

    // if (fileName.isEmpty()) {
    //     return nullptr;
    // }
    QString fileName = this->dirPath.absolutePath()+"/"+lastCreatedFileName;
    // qDebug() << fileName << __FUNCTION__;
    if (!fileName.endsWith(".nmea", Qt::CaseInsensitive)) {
        fileName += ".nmea";
    }

    Connection* new_connection = nullptr;

    if (ui->connection_types->currentIndex() == 0) { // net connection
        QString IP = ui->lineEdit->text();
        uint16_t port = ui->lineEdit_2->text().toInt();
        qDebug() << "Connection created" << IP << port;
        if (ui->TCP_RB->isChecked()) {
            new_connection = new Connection_Net(IP, port, fileName);
        } else {
            new_connection = new connection_UDP(IP, port, fileName);
        }
    } else { // COM connection
        QString comport = ui->PortsAvailableComboBox->currentText();
        qDebug() << "COM Port: " << comport;
        qDebug() << "Baud Rate: " << ui->BRate_choose->currentText().toInt();
        new_connection = new Connection_com(comport, ui->BRate_choose->currentText().toInt(), fileName);
        qDebug() << "COM Connection created" << comport << ui->BRate_choose->currentText().toInt();
    }

    lastConnection = new_connection;

    return new_connection;
}

Connection *Connection_creator::getLastConnection()
{
    return lastConnection;
}

void Connection_creator::on_connection_types_currentChanged(int index)
{
    if (index == 0) {
        ui->pushButton->setEnabled(true);
    } else {
        if (ui->PortsAvailableComboBox->count() == 0) {
            ui->pushButton->setEnabled(false);
        }
    }
}

void Connection_creator::setLastCreatedFileName(const QString &newLastCreatedFileName)
{
    lastCreatedFileName = newLastCreatedFileName;
}

void Connection_creator::setDirPath(const QDir &newDirPath)
{
    dirPath = newDirPath;
}



