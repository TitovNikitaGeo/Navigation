#include "connection_creator.h"
#include "ui_connection_creator.h"

Connection_creator::Connection_creator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Connection_creator)
{
    ui->setupUi(this);
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();


    for (QSerialPortInfo port: ports) { //получаем список доступных COM портов
        ui->PortsAvailableComboBox->addItem(port.portName());
    }

    //if no COM-ports are available
    if (ports.empty() && ui->connection_types->currentIndex() == 1) {
        // qDebug() << "No COM ports available";
        ui->Error_label_COM_tab->setStyleSheet("QLabel {color : red; }");
        ui->Error_label_COM_tab->setText("No COM ports available");
        ui->pushButton->setEnabled(false);
    }
}

Connection_creator::~Connection_creator()
{
    qDebug() << "Connection_creator killed";
    for (auto connection: Connection_vector) {
        if (connection) delete connection;
    }
    delete ui;

}


///QML black magic
void Connection_creator::on_pushButton_clicked()
{
    Connection* newConnection = createConnection();
    emit(connectionCreated(newConnection));
}

Connection* Connection_creator::createConnection(){
    ///choose the location in future if needed
    QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                    "Save NMEA File",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/tests_nmea",
                                                    "NMEA Files (*.nmea);;All Files (*)",
                                                    nullptr,
                                                    QFileDialog::DontConfirmOverwrite);
    if (fileName.isEmpty()) return nullptr;
    if (!fileName.endsWith(".nmea", Qt::CaseInsensitive))
    {
        fileName += ".nmea";
    }

    Connection* new_connection;

    if (ui->connection_types->currentIndex() == 0) { //connection TCP
        QString IP = ui->lineEdit->text();
        uint16_t port = ui->lineEdit_2->text().toInt();
        qDebug() << "Connection created" << IP << port;
        new_connection = new Connection_Net(IP, port, fileName);
        new_connection->create_file_for_nmea(fileName);
        new_connection->show();
        Connection_vector.append(new_connection);
    } else {
        QString comport = ui->PortsAvailableComboBox->currentText();
        qDebug() << comport;
        qDebug() << ui->BRate_choose->currentText().toInt();

        new_connection = new Connection_com(
            comport, ui->BRate_choose->currentText().toInt(), fileName);
        qDebug() << "Connection created" << comport  << ui->BRate_choose->currentText().toInt();
        // serial.setPortName("COM"+QString::number(ui->COM_number_spin_box->value()));
        new_connection->show();;
    }
    return new_connection;

}

