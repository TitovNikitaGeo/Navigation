/********************************************************************************
** Form generated from reading UI file 'connection.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTION_H
#define UI_CONNECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Connection
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Connection)
    {
        if (Connection->objectName().isEmpty())
            Connection->setObjectName("Connection");
        Connection->resize(636, 144);
        label = new QLabel(Connection);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 40, 601, 71));
        label_2 = new QLabel(Connection);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(300, 110, 49, 16));

        retranslateUi(Connection);

        QMetaObject::connectSlotsByName(Connection);
    } // setupUi

    void retranslateUi(QWidget *Connection)
    {
        Connection->setWindowTitle(QCoreApplication::translate("Connection", "Form", nullptr));
        label->setText(QCoreApplication::translate("Connection", "EMPTY", nullptr));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Connection: public Ui_Connection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTION_H
