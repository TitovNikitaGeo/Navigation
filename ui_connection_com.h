/********************************************************************************
** Form generated from reading UI file 'connection_com.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTION_COM_H
#define UI_CONNECTION_COM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Connection_com
{
public:

    void setupUi(QWidget *Connection_com)
    {
        if (Connection_com->objectName().isEmpty())
            Connection_com->setObjectName("Connection_com");
        Connection_com->resize(682, 198);

        retranslateUi(Connection_com);

        QMetaObject::connectSlotsByName(Connection_com);
    } // setupUi

    void retranslateUi(QWidget *Connection_com)
    {
        Connection_com->setWindowTitle(QCoreApplication::translate("Connection_com", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Connection_com: public Ui_Connection_com {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTION_COM_H
