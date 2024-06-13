/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_4;
    QPushButton *AddItemtPushButton;
    QLineEdit *YLineEdit;
    QLineEdit *ZLineEdit;
    QLabel *label_2;
    QPushButton *DeleteItemPushButton;
    QLabel *label_5;
    QLabel *label_3;
    QLineEdit *XLineEdit;
    QLineEdit *ItemNameLineEdit;
    QWidget *graphicsViewContainer;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *RBFixed;
    QRadioButton *RBTowed;
    QTableWidget *ItemListTable;
    QLabel *label;
    QDoubleSpinBox *WireLengthSpinBox;
    QLabel *label_6;
    QComboBox *ComboBoxWiredWith;
    QLabel *label_7;
    QComboBox *ComboBoxItemType;
    QCheckBox *checkBox;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(896, 524);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 65, 91, 16));
        QFont font;
        font.setPointSize(12);
        label_4->setFont(font);
        AddItemtPushButton = new QPushButton(centralwidget);
        AddItemtPushButton->setObjectName("AddItemtPushButton");
        AddItemtPushButton->setGeometry(QRect(10, 250, 121, 41));
        YLineEdit = new QLineEdit(centralwidget);
        YLineEdit->setObjectName("YLineEdit");
        YLineEdit->setGeometry(QRect(140, 60, 131, 24));
        ZLineEdit = new QLineEdit(centralwidget);
        ZLineEdit->setObjectName("ZLineEdit");
        ZLineEdit->setGeometry(QRect(140, 90, 131, 24));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 5, 91, 16));
        label_2->setFont(font);
        DeleteItemPushButton = new QPushButton(centralwidget);
        DeleteItemPushButton->setObjectName("DeleteItemPushButton");
        DeleteItemPushButton->setGeometry(QRect(140, 250, 131, 41));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 95, 91, 16));
        label_5->setFont(font);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 35, 91, 16));
        label_3->setFont(font);
        XLineEdit = new QLineEdit(centralwidget);
        XLineEdit->setObjectName("XLineEdit");
        XLineEdit->setGeometry(QRect(140, 30, 131, 24));
        ItemNameLineEdit = new QLineEdit(centralwidget);
        ItemNameLineEdit->setObjectName("ItemNameLineEdit");
        ItemNameLineEdit->setGeometry(QRect(140, 0, 131, 24));
        graphicsViewContainer = new QWidget(centralwidget);
        graphicsViewContainer->setObjectName("graphicsViewContainer");
        graphicsViewContainer->setGeometry(QRect(340, 0, 541, 491));
        verticalLayoutWidget = new QWidget(graphicsViewContainer);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 0, 541, 491));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        RBFixed = new QRadioButton(centralwidget);
        RBFixed->setObjectName("RBFixed");
        RBFixed->setGeometry(QRect(10, 120, 91, 22));
        RBFixed->setChecked(true);
        RBTowed = new QRadioButton(centralwidget);
        RBTowed->setObjectName("RBTowed");
        RBTowed->setGeometry(QRect(80, 120, 91, 22));
        RBTowed->setChecked(false);
        ItemListTable = new QTableWidget(centralwidget);
        ItemListTable->setObjectName("ItemListTable");
        ItemListTable->setGeometry(QRect(10, 300, 261, 191));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 150, 101, 16));
        label->setFont(font);
        WireLengthSpinBox = new QDoubleSpinBox(centralwidget);
        WireLengthSpinBox->setObjectName("WireLengthSpinBox");
        WireLengthSpinBox->setGeometry(QRect(140, 150, 131, 25));
        WireLengthSpinBox->setMaximum(5000.000000000000000);
        WireLengthSpinBox->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        WireLengthSpinBox->setValue(100.000000000000000);
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 185, 101, 16));
        label_6->setFont(font);
        ComboBoxWiredWith = new QComboBox(centralwidget);
        ComboBoxWiredWith->setObjectName("ComboBoxWiredWith");
        ComboBoxWiredWith->setGeometry(QRect(140, 180, 131, 24));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(10, 210, 101, 21));
        label_7->setFont(font);
        ComboBoxItemType = new QComboBox(centralwidget);
        ComboBoxItemType->addItem(QString());
        ComboBoxItemType->addItem(QString());
        ComboBoxItemType->addItem(QString());
        ComboBoxItemType->addItem(QString());
        ComboBoxItemType->setObjectName("ComboBoxItemType");
        ComboBoxItemType->setGeometry(QRect(140, 210, 131, 24));
        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(170, 120, 101, 22));
        checkBox->setChecked(true);
        MainWindow->setCentralWidget(centralwidget);
        RBFixed->raise();
        label_4->raise();
        AddItemtPushButton->raise();
        YLineEdit->raise();
        ZLineEdit->raise();
        label_2->raise();
        DeleteItemPushButton->raise();
        label_5->raise();
        label_3->raise();
        XLineEdit->raise();
        ItemNameLineEdit->raise();
        graphicsViewContainer->raise();
        RBTowed->raise();
        ItemListTable->raise();
        label->raise();
        WireLengthSpinBox->raise();
        label_6->raise();
        ComboBoxWiredWith->raise();
        label_7->raise();
        ComboBoxItemType->raise();
        checkBox->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 896, 21));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        AddItemtPushButton->setText(QCoreApplication::translate("MainWindow", "Add Item", nullptr));
        YLineEdit->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        ZLineEdit->setText(QCoreApplication::translate("MainWindow", "-10", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Item Name", nullptr));
        DeleteItemPushButton->setText(QCoreApplication::translate("MainWindow", "Delete Item", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Z", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        XLineEdit->setText(QCoreApplication::translate("MainWindow", "-100", nullptr));
        ItemNameLineEdit->setText(QCoreApplication::translate("MainWindow", "A-Crane", nullptr));
        RBFixed->setText(QCoreApplication::translate("MainWindow", "Fixed", nullptr));
        RBTowed->setText(QCoreApplication::translate("MainWindow", "Towed", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Wire Length", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Wired With", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Item Type", nullptr));
        ComboBoxItemType->setItemText(0, QCoreApplication::translate("MainWindow", "Towed", nullptr));
        ComboBoxItemType->setItemText(1, QCoreApplication::translate("MainWindow", "Source", nullptr));
        ComboBoxItemType->setItemText(2, QCoreApplication::translate("MainWindow", "Buoy", nullptr));
        ComboBoxItemType->setItemText(3, QCoreApplication::translate("MainWindow", "Streamer", nullptr));

        checkBox->setText(QCoreApplication::translate("MainWindow", "Connection", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
