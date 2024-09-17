/********************************************************************************
** Form generated from reading UI file 'connection_creator.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTION_CREATOR_H
#define UI_CONNECTION_CREATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Connection_creator
{
public:
    QWidget *centralwidget;
    QTabWidget *connection_types;
    QWidget *IP;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit;
    QLabel *label;
    QWidget *COM;
    QLabel *Com_Label;
    QComboBox *BRate_choose;
    QLabel *BaudRateLabel;
    QComboBox *PortsAvailableComboBox;
    QLabel *Error_label_COM_tab;
    QPushButton *pushButton;

    void setupUi(QDialog *Connection_creator)
    {
        if (Connection_creator->objectName().isEmpty())
            Connection_creator->setObjectName("Connection_creator");
        Connection_creator->resize(257, 198);
        centralwidget = new QWidget(Connection_creator);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setGeometry(QRect(10, 10, 221, 171));
        connection_types = new QTabWidget(centralwidget);
        connection_types->setObjectName("connection_types");
        connection_types->setGeometry(QRect(0, 0, 221, 131));
        IP = new QWidget();
        IP->setObjectName("IP");
        label_2 = new QLabel(IP);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(150, 60, 49, 16));
        lineEdit_2 = new QLineEdit(IP);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(10, 60, 113, 24));
        lineEdit = new QLineEdit(IP);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(10, 20, 113, 24));
        label = new QLabel(IP);
        label->setObjectName("label");
        label->setGeometry(QRect(150, 20, 49, 16));
        connection_types->addTab(IP, QString());
        COM = new QWidget();
        COM->setObjectName("COM");
        Com_Label = new QLabel(COM);
        Com_Label->setObjectName("Com_Label");
        Com_Label->setGeometry(QRect(10, 15, 49, 16));
        BRate_choose = new QComboBox(COM);
        BRate_choose->addItem(QString());
        BRate_choose->addItem(QString());
        BRate_choose->addItem(QString());
        BRate_choose->addItem(QString());
        BRate_choose->addItem(QString());
        BRate_choose->setObjectName("BRate_choose");
        BRate_choose->setGeometry(QRect(100, 40, 72, 24));
        BaudRateLabel = new QLabel(COM);
        BaudRateLabel->setObjectName("BaudRateLabel");
        BaudRateLabel->setGeometry(QRect(10, 45, 61, 16));
        PortsAvailableComboBox = new QComboBox(COM);
        PortsAvailableComboBox->setObjectName("PortsAvailableComboBox");
        PortsAvailableComboBox->setGeometry(QRect(100, 10, 72, 24));
        Error_label_COM_tab = new QLabel(COM);
        Error_label_COM_tab->setObjectName("Error_label_COM_tab");
        Error_label_COM_tab->setGeometry(QRect(10, 70, 201, 21));
        connection_types->addTab(COM, QString());
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 140, 211, 31));

        retranslateUi(Connection_creator);

        connection_types->setCurrentIndex(0);
        BRate_choose->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(Connection_creator);
    } // setupUi

    void retranslateUi(QDialog *Connection_creator)
    {
        Connection_creator->setWindowTitle(QCoreApplication::translate("Connection_creator", "Connection_creator", nullptr));
        label_2->setText(QCoreApplication::translate("Connection_creator", "Port", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("Connection_creator", "1001", nullptr));
        lineEdit->setText(QCoreApplication::translate("Connection_creator", "127.0.0.1", nullptr));
        label->setText(QCoreApplication::translate("Connection_creator", "IP", nullptr));
        connection_types->setTabText(connection_types->indexOf(IP), QCoreApplication::translate("Connection_creator", "IP", nullptr));
        Com_Label->setText(QCoreApplication::translate("Connection_creator", "COM", nullptr));
        BRate_choose->setItemText(0, QCoreApplication::translate("Connection_creator", "9600", nullptr));
        BRate_choose->setItemText(1, QCoreApplication::translate("Connection_creator", "19200", nullptr));
        BRate_choose->setItemText(2, QCoreApplication::translate("Connection_creator", "38400", nullptr));
        BRate_choose->setItemText(3, QCoreApplication::translate("Connection_creator", "57600", nullptr));
        BRate_choose->setItemText(4, QCoreApplication::translate("Connection_creator", "115200", nullptr));

        BRate_choose->setCurrentText(QCoreApplication::translate("Connection_creator", "57600", nullptr));
        BaudRateLabel->setText(QCoreApplication::translate("Connection_creator", "BaudRate", nullptr));
        Error_label_COM_tab->setText(QString());
        connection_types->setTabText(connection_types->indexOf(COM), QCoreApplication::translate("Connection_creator", "COM", nullptr));
        pushButton->setText(QCoreApplication::translate("Connection_creator", "Create connection", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Connection_creator: public Ui_Connection_creator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTION_CREATOR_H
