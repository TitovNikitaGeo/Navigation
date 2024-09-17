/********************************************************************************
** Form generated from reading UI file 'connection_net.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTION_NET_H
#define UI_CONNECTION_NET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Connection_Net
{
public:

    void setupUi(QWidget *Connection_Net)
    {
        if (Connection_Net->objectName().isEmpty())
            Connection_Net->setObjectName("Connection_Net");
        Connection_Net->resize(810, 178);

        retranslateUi(Connection_Net);

        QMetaObject::connectSlotsByName(Connection_Net);
    } // setupUi

    void retranslateUi(QWidget *Connection_Net)
    {
        Connection_Net->setWindowTitle(QCoreApplication::translate("Connection_Net", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Connection_Net: public Ui_Connection_Net {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTION_NET_H
