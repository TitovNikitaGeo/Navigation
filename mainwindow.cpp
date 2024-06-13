#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // MyTimer = new QTimer(this);
    // connect(MyTimer, &QTimer::timeout, this, &MainWindow::timeToCollectData);
    // MyTimer->start(1000);



    ///Just for init settings of widgets
    emit on_RBFixed_clicked();
    ///Just for init settings of widgets


    ///pointer to our list with item information
    tableWithItems = ui->ItemListTable;
    ///pointer to our list with item information

    ///Settings for MyGraphicView
    DrawingArea = new MyGraphicView(this);
    ui->verticalLayout->addWidget(DrawingArea);
    ///Settings for MyGraphicView

    ///Fabric initialization
    MyFabric = new Fabric();
    ///Fabric initialization

    ///Vault initialization
    Vault = new ItemsStorage();
    ///Vault initialization

    ///ObjectsListSetUp
    setUpObjectsTable();
    ///ObjectsListSetUp


    connectionCreator = new Connection_creator();
    connect(connectionCreator, &Connection_creator::connectionCreated, this, &MainWindow::handleConnection);
}

void MainWindow::timeToCollectData()
{
    for (FixedItem* item: Vault->ItemsVault) {
        if (item->connection != nullptr) {
            if (!item->hasConnection) {
                qDebug() << "MW time to collect";
                return;
            } else {
                item->newNmeaArived(item->getLastNmeaStr());
                item->getLastNmeaStr();
            }
        }
    }
    qDebug()<<"timeToCollectData tic tac";

}

MainWindow::~MainWindow()
{
    delete MyFabric;
    delete DrawingArea;
    delete Vault;
    delete connectionCreator;
    delete ui;
}


void MainWindow::on_DeleteItemPushButton_clicked()
{
    int row = tableWithItems->currentRow();
    if (row == -1) row = tableWithItems->rowCount() - 1;

    QString deleteName = tableWithItems->item(row, 0)->text();
    if (row >= 0) {
        tableWithItems->removeRow(row);
        Vault->DeleteItem(deleteName);
    } else {
        QMessageBox msgBox;
        msgBox.critical(this, "Error","Нечего удалять");
        return;
    }
}


void MainWindow::on_AddItemtPushButton_clicked()
{
    ///Creating New Obj
    QString oldname = ui->ItemNameLineEdit->text();
    QString name = GetNewDeviceName(oldname);
    ui->ItemNameLineEdit->setText(name);
    ///Creating New Obj

    FixedItem* newItem;

    if (ui->RBFixed->isChecked()) { ///getting params for fixed
        float x = ui->XLineEdit->text().toFloat();
        float y = ui->YLineEdit->text().toFloat();
        float z = ui->ZLineEdit->text().toFloat();
        Fabric::FixedItemInfo NewItemInfo(x,y,z,name);
        newItem = createFixedItem(NewItemInfo);
    }
    else if (ui->RBTowed->isChecked()) { ///getting params for towed
        if(ui->ComboBoxWiredWith->count() == 0) {
            QMessageBox msgBox;
            msgBox.critical(this, "Error","Нет точек крепления");
            return;
        }
        FixedItem* twiw = Vault->getItem(
            ui->ComboBoxWiredWith->currentText());//toWhoIsWired
        float wireLength = ui->WireLengthSpinBox->value();
        float angleToWired = 270; //change further if needed
        //270 градусов = 4.71238898
        Fabric::TowedItemInfo NewItemInfo(twiw, wireLength, angleToWired, name);
        newItem = createTowedItem(NewItemInfo);
    }

    /// открываем окно создания соединений, если нужно
    if (ui->checkBox->isChecked()) {
        connectionCreator->show();
        newItem->connection = getConnection();
        newItem->hasConnection = true;
        //получение указателя на соединение
    } else {
        newItem->connection = nullptr;
    }
}


void MainWindow::setUpObjectsTable() {
    tableWithItems->setColumnCount(4);

    tableWithItems->setHorizontalHeaderLabels({"Name", "X", "Y", "Z"});
    tableWithItems->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableWithItems->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableWithItems->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    tableWithItems->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWithItems->setSelectionMode(QAbstractItemView::SingleSelection);

    tableWithItems->setEditTriggers(QTableWidget::NoEditTriggers);
}



void MainWindow::on_RBFixed_clicked()
{
    ui->ComboBoxItemType->setDisabled(true);
    ui->ComboBoxWiredWith->setDisabled(true);
    ui->WireLengthSpinBox->setDisabled(true);

    ui->XLineEdit->setDisabled(false);
    ui->YLineEdit->setDisabled(false);
    ui->ZLineEdit->setDisabled(false);
}


void MainWindow::on_RBTowed_clicked()
{
    ui->ComboBoxItemType->setDisabled(false);
    ui->ComboBoxWiredWith->setDisabled(false);
    ui->WireLengthSpinBox->setDisabled(false);

    ui->XLineEdit->setDisabled(true);
    ui->YLineEdit->setDisabled(true);
    ui->ZLineEdit->setDisabled(true);
}


FixedItem* MainWindow::createFixedItem(Fabric::FixedItemInfo NewItemInfo) {

    FixedItem* NewItem = MyFabric->CreateFixedItem(NewItemInfo);

    ///Saving New Item
    Vault->SaveFixedItem(NewItem);
    ///Saving New Item

    ///adding obj to table
    addItemToObjectsList(NewItem);
    ///adding obj to table

    ///For now is only for fixed
    ui->ComboBoxWiredWith->addItem(NewItem->name);
    ///For now is only for fixed

    ///adding new object to our Drawing area
    DrawingArea->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    ///adding new object to our Drawing area

    return NewItem;
}

FixedItem* MainWindow::createTowedItem(Fabric::TowedItemInfo NewItemInfo) {

    TowedItem* NewItem = MyFabric->CreateTowedItem(NewItemInfo);

    ///Saving New Item
    Vault->SaveFixedItem(NewItem);
    ///Saving New Item


    ///adding obj to table
    addItemToObjectsList(NewItem);
    ///adding obj to table

    // ///For now is only for fixed
    // ui->ComboBoxWiredWith->addItem(NewItem->name);
    // ///For now is only for fixed

    ///adding new object to our Drawing area
    DrawingArea->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    drawLineToTowed(NewItem);
    ///adding new object to our Drawing area


    return NewItem;
}

void MainWindow::addItemToObjectsList(FixedItem* newItem) {

    QString name = newItem->name;
    // QString type = newItem->itemType;
    float x = newItem->x;
    float y = newItem->y;
    float z = newItem->z;
    tableWithItems->insertRow(NumberOfRowsInTable);
    tableWithItems->setItem(NumberOfRowsInTable, 0, new QTableWidgetItem(name));
    tableWithItems->setItem(NumberOfRowsInTable, 1, new QTableWidgetItem(QString::number(x,'g',10)));
    tableWithItems->setItem(NumberOfRowsInTable, 2, new QTableWidgetItem(QString::number(y,'g',10)));
    tableWithItems->setItem(NumberOfRowsInTable, 3, new QTableWidgetItem(QString::number(z,'g',10)));
    NumberOfRowsInTable++;
    // QString StringToPresentItem = name + QString::number(x) + QString::number(y);

}


QString MainWindow::GetNewDeviceName(QString name){
    //делает невозможным повторение имен
    //если имена повторяются, добавляет постфикс _1 _2 _3 и тд
    for (FixedItem* item: Vault->ItemsVault) {
        if (name == item->name) {
            if (name.at(name.length()-1).isDigit()){
                int curNumber = name.mid(1+name.lastIndexOf("_")).toInt();
                return name.left(name.lastIndexOf("_")) + "_" + QString::number(curNumber+1);
            } else {
                return name + "_1";
            }
        }
    }
    return name;
}

void MainWindow::drawLineToTowed(TowedItem* item) {
    //рисует линию от буксируемого устройства к точке крепления
    DrawingArea->drawLineToTowed(item->x, item->y,
            item->towingPoint->x, item->towingPoint->y);
}



///костыль или архитектура? Вот в чем вопрос
Connection* MainWindow::getConnection(){
     qDebug() << "MW get connection" << lastCreatedConnection->filename;
    return lastCreatedConnection;
}
void MainWindow::handleConnection(Connection* newConnection){

    lastCreatedConnection = newConnection;
    connectionCreator->hide();
}
///костыль или архитектура? Вот в чем вопрос














