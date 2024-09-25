#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MyTimer = new QTimer(this);
    connect(MyTimer, &QTimer::timeout, this, &MainWindow::timeToCollectData);
    // MyTimer->start(1000);

    ///Just for init settings of widgets
    emit on_RBFixed_clicked();
    ///Just for init settings of widgets


    ///pointer to our list with item information
    tableWithItems = ui->ItemListTable;
    ///pointer to our list with item information


    ///Settings for MyGraphicView
    DrawingAreaTopView = new MyGraphicView(this);
    DrawingAreaSideView = new SideGraphicView(this);
    ui->verticalLayout->addWidget(DrawingAreaTopView);
    ui->verticalLayout->addWidget(DrawingAreaSideView);
    DrawingAreaSideView->hide();
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

    ///coordinator creation
    coordinator = new Coordinator();
    coordinator->Vault = this->Vault;
    ///coordinator creation

    ///p190Creator
    p190Creator = new P190_creator();
    p190Creator->setItemStoragePtr(Vault);

    ///p190Creator


    ///for boardHeight setting
    connect(ui->boardHeightSpinBox, &QDoubleSpinBox::valueChanged,
            coordinator,&Coordinator::boardDepthChanged);

    setMenuBar(this->menuBar());
    setWindowTitle("Seismic Navigation");



    ///one path to rule all the files
    ///and logger
    logger = (&Logger::instance());
    setPathForAllFiles();    
    logger->createLogFile();

    ///КоСТЫЛЬ ААААААААААААААААААА
    setSegyReader();
}
void MainWindow::timeToCollectData()
{
    static NmeaParser p;
    int FFID = pairs[curSegyFile].ffid;
    QTime TIME = pairs[curSegyFile].time;
    for (auto i: Vault->ItemsVault) {
        i->calcItemCoordinates();
        if (TIME.msecsTo(p.parseNmeaGGA(i->connection->lastRecievedGGA).dateTime.time()) < 1000) {
            p190Creator->setFFID(FFID);
            p190Creator->createShotBlock();
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    // log->logMessage(this, Logger::Info, "Main Window destructor");
    delete MyFabric;
    delete DrawingAreaTopView;
    delete Vault;
    delete ui;
    // logger->
    exit(0);
}


void MainWindow::on_DeleteItemPushButton_clicked()
{
    int row = tableWithItems->currentRow();
    if (tableWithItems->rowCount() == 0) {
        QMessageBox msgBox;
        msgBox.critical(this, "Error","Нечего удалять");
        return;
    }
    if (row == -1) row = tableWithItems->rowCount() - 1;
    QString deleteName = tableWithItems->item(row, 0)->text();
    if (row >= 0) {
        tableWithItems->removeRow(row);
        Vault->DeleteItem(deleteName);
    }
}


void MainWindow::on_AddItemtPushButton_clicked()
{
    ///Creating New Obj
    QString oldname = ui->ItemNameLineEdit->text();
    QString name = GetNewDeviceName(oldname);
    ui->ItemNameLineEdit->setText(name);
    ///Creating New Obj

    // FixedItem* newItem;

    if (ui->RBFixed->isChecked()) { ///getting params for fixed
        float x = ui->XLineEdit->text().toFloat();
        float y = ui->YLineEdit->text().toFloat();
        float z = ui->ZLineEdit->text().toFloat();
        FixedItemInfo FixedItemInfo(x,y,z,name);
        //newItem =
        createFixedItem(FixedItemInfo);
    } else if (ui->RBTowed->isChecked()) {
        ///getting params for towed
        if(ui->ComboBoxWiredWith->count() == 0) {
            QMessageBox msgBox;
            msgBox.critical(this, "Error","Нет точек крепления");
            return;
        }

        FixedItem* twiw = Vault->getItem(
            ui->ComboBoxWiredWith->currentText());//toWhoIsWired
        float wireLength = ui->WireLengthSpinBox->value();
        float angleToWired = 270; //change further if needed
        QString itemType = ui->ComboBoxItemType->currentText();
        TowedItemInfo TowedItemInfo(twiw, wireLength, angleToWired, name);
        TowedItem* towedItem;

        if (itemType == "Streamer") {
            StreamerInfo StreamerItemInfo{TowedItemInfo, 0, {0}};
            // Streamer* newItem =
            towedItem = createStreamerItem(StreamerItemInfo);
        } else if (itemType == "Buoy") {
            MyFabric->setMyVault(Vault);
            BuoyInfo BuoyItemInfo{TowedItemInfo, 0,0};
            towedItem = createBuoyItem(BuoyItemInfo);
        } else if (itemType == "Towed") {
            //newItem =
            towedItem = createTowedItem(TowedItemInfo);
        } else if (itemType == "Source") {
            SourceInfo SourceItemInfo = {TowedItemInfo};
            towedItem = createSourceItem(SourceItemInfo);
        }
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

void MainWindow::setMenuBar(QMenuBar *menuBar)
{
    QMenu *fileMenu = menuBar->findChild<QMenu*>("fileMenu");
    if (!fileMenu) {
        // Если нет, создаем новое меню "Файл"
        fileMenu = new QMenu("File", menuBar);
        fileMenu->setObjectName("fileMenu"); // Присваиваем имя для поиска в будущем
        menuBar->addMenu(fileMenu);
    } else {
        // Если меню уже существует, очищаем его для добавления новых действий
        fileMenu->clear();
    }

    // Создание действия "Сохранить параметры"
    QAction *saveAction = new QAction("Save configuration", fileMenu);
    QObject::connect(saveAction, &QAction::triggered, menuBar->parent(), [this]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save configuration", "", "Config Files (*.json);;All Files (*)");
        if (!fileName.isEmpty()) {
            // Логика сохранения параметров в файл
            QMessageBox::information(nullptr, "Save configuration", QString("Configuration saved at %1").arg(fileName));
            QJsonObject params = ItemsLoader::instance().
                     writeToJSON(this->Vault->ItemsVault);
            ItemsLoader::saveJsonObjectToFile(params, fileName);
        }
    });
    fileMenu->addAction(saveAction);

    // Создание действия "Загрузить параметры"
    QAction *loadAction = new QAction("Load configuration", fileMenu);
    QObject::connect(loadAction, &QAction::triggered, menuBar->parent(), []() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Load configuration", "", "Config Files (*.json);;All Files (*)");
        if (!fileName.isEmpty()) {
            // Логика загрузки параметров из файла
            QMessageBox::information(nullptr, "Load configuration", QString("Configuration loaded from %1").arg(fileName));
        }
    });
    fileMenu->addAction(loadAction);

    // Создание действия "Постпроцессинг"
    QAction *postProcessingAction = new QAction("Start postprocessing", fileMenu);
    QObject::connect(postProcessingAction, &QAction::triggered, menuBar->parent(), [this]() {
        QString dirWithNmea = QFileDialog::getExistingDirectory(
            nullptr, "Select folder with raw Nmea data", pathForAllFiles.absolutePath(),           // Default directory
            QFileDialog::DontResolveSymlinks);
        QString dirWithSegy = QFileDialog::getExistingDirectory(nullptr, "Select folder with Seg-Y",
        pathForAllFiles.absolutePath(),           // Default directory
        QFileDialog::DontResolveSymlinks);
        postProcessing(dirWithNmea, dirWithSegy);

    });
    fileMenu->addAction(postProcessingAction);

}

void MainWindow::setPathForAllFiles()
{
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString folderName = QString("%1_%2__%3_%4")
                             .arg(currentDateTime.time().hour())
                             .arg(currentDateTime.time().minute(), 2, 10, QChar('0'))
                             .arg(currentDateTime.date().day(),2, 10, QChar('0'))
                             .arg(currentDateTime.date().month(),2, 10, QChar('0'));


    QDir dir(dirPath + "/Ship_logs/" + folderName);
    dir.mkpath(dirPath + "/Ship_logs/" + folderName);
    this->pathForAllFiles = dir;

    MyFabric->connectionCreator->setDirPath(dir);
    p190Creator->setPath(dir);
    logger->setPath(dir);
}

void MainWindow::loadConfig()
{


}

void MainWindow::saveConfig()
{

}

void MainWindow::postProcessing(QDir pathNmea, QDir pathSegy)
{
    PostProcessor processor;
    processor.setP190(this->p190Creator);
    processor.setNmeaStorage(pathNmea);
    processor.setSegyStorage(pathSegy);
    processor.getDataFromSegy();
    processor.setMyVault(this->Vault);
}

void MainWindow::setSegyReader()
{
    sr = new SegYReader();
    sr->readPathWithSegy(QDir("C:\\Users\\sabrahar\\Desktop\\FINAL\\test20000-30000"));
    for (auto i:sr->pairs) {
        pairs.append(i);
        qDebug() << i.ffid << i.time;
    }

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


FixedItem* MainWindow::createFixedItem(FixedItemInfo NewItemInfo) {
    bool needConnection = false;

    if (ui->NeedConnectionCB->isChecked()) {
        needConnection = true;
    }


    FixedItem* NewItem = MyFabric->CreateItem(NewItemInfo, needConnection);

    ///Saving New Item
    Vault->SaveItem(NewItem);
    ///Saving New Item

    ///adding obj to table
    addItemToObjectsList(NewItem);
    ///adding obj to table

    ///For now is only for fixed
    ui->ComboBoxWiredWith->addItem(NewItem->name);
    ///For now is only for fixed

    ///adding new object to our Drawing area
    DrawingAreaTopView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    DrawingAreaSideView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    ///adding new object to our Drawing area

    return NewItem;
}

TowedItem* MainWindow::createTowedItem(TowedItemInfo NewItemInfo) {
    bool needConnection = false;

    if (ui->NeedConnectionCB->isChecked()) {
        needConnection = true;
    }
    TowedItem* NewItem = MyFabric->CreateItem(NewItemInfo, needConnection);
    if (NewItem == nullptr) return nullptr;
    ///Saving New Item
    Vault->SaveItem(NewItem);
    ///Saving New Item


    ///adding obj to table
    addItemToObjectsList(NewItem);
    ///adding obj to table

    ///adding ability to wire any towed item with it
    ui->ComboBoxWiredWith->addItem(NewItem->name);

    ///adding new object to our Drawing area
    DrawingAreaTopView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    DrawingAreaSideView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    drawLineToTowed(NewItem);
    ///adding new object to our Drawing area


    return NewItem;
}

Streamer* MainWindow::createStreamerItem(StreamerInfo info)
{
    // QString name = info.towedInfo.name;
    // FixedItem* twiw = info.towedInfo.toWhoIsWired;

    // Streamer* NewItem = new Streamer();
    Streamer* NewItem = MyFabric->CreateItem(info);
    if (NewItem == nullptr) return nullptr;

    ///Saving New Item
    Vault->SaveItem(NewItem);
    ///Saving New Item

    ///adding obj to table
    addItemToObjectsList(NewItem);
    ///adding obj to table

    ///adding ability to wire any towed item with it
    ui->ComboBoxWiredWith->addItem(NewItem->name);

    ///Drawing
    drawLineToTowed(NewItem);
    DrawingAreaSideView->drawStreamer(NewItem->x, NewItem->y,
        NewItem->getChan(NewItem->getChanCount()-1)->x,
        NewItem->getChan(NewItem->getChanCount()-1)->y,
        NewItem->getChanCount());
    DrawingAreaSideView->drawStreamer(NewItem->x, NewItem->z,
        NewItem->getChan(NewItem->getChanCount()-1)->x,
        NewItem->getChan(NewItem->getChanCount()-1)->z,
        NewItem->getChanCount());


    return NewItem;


}

Buoy* MainWindow::createBuoyItem(BuoyInfo BuoyItemInfo)
{
    bool needConnection = false;
    if (ui->NeedConnectionCB->isChecked()) {
        needConnection = true;
    }

    Buoy* NewItem = MyFabric->CreateItem(BuoyItemInfo, needConnection);
    if (QString(BuoyItemInfo.towedInfo.toWhoIsWired->metaObject()->className()) == "Streamer") {
        dynamic_cast<Streamer*>(Vault->getItem(BuoyItemInfo.towedInfo.toWhoIsWired->name))->setEndBuoy(NewItem);
        NewItem->wireLength += dynamic_cast<Streamer*>(Vault->getItem(BuoyItemInfo.towedInfo.toWhoIsWired->name))->getTotalLength();
    }
    //тут мы проверяем, что если буй привязан к косе, то коса->концевой буй* = новый созданный объект
    //а потом увеличиваем дистанцию буксировки на длину косы


    ///Saving New Item
    Vault->SaveItem(NewItem);
    ///Saving New Item

    ///adding obj to table
    addItemToObjectsList(NewItem);
    ///adding obj to table

    ///adding ability to wire any towed item with it
    ui->ComboBoxWiredWith->addItem(NewItem->name);

    //drawing
    drawLineToTowed(NewItem);
    DrawingAreaTopView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    DrawingAreaSideView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);

    return NewItem;
}

Source* MainWindow::createSourceItem(SourceInfo SourceItemInfo) //метод для создания объекта под источник
{
    bool needConnection = false;
    if (ui->NeedConnectionCB->isChecked()) {
        needConnection = true;
    }


    Source* NewItem = MyFabric->CreateItem(SourceItemInfo, needConnection);
    ///Saving New Item
    Vault->SaveItem(NewItem);
    ///Saving New Item

    ///adding obj to table
    addItemToObjectsList(NewItem);
    ///adding obj to table

    ///adding ability to wire any towed item with it
    ui->ComboBoxWiredWith->addItem(NewItem->name);

    //drawing
    drawLineToTowed(NewItem);
    DrawingAreaTopView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);
    DrawingAreaSideView->addPoint(NewItem->x,NewItem->y,NewItem->z,NewItem->name);

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
    if (QString(item->towingPoint->metaObject()->className()) == "Streamer") {
        float streamerLength = dynamic_cast<Streamer*>(item->towingPoint)->totalLength;
        DrawingAreaTopView->drawLineToTowed(item->x, item->y,
                                            item->towingPoint->x-streamerLength, item->towingPoint->y);
        DrawingAreaSideView->drawLineToTowed(item->x, item->z,
                                             item->towingPoint->x-streamerLength, item->towingPoint->z);
    } else {
        DrawingAreaTopView->drawLineToTowed(item->x, item->y,
            item->towingPoint->x, item->towingPoint->y);
        DrawingAreaSideView->drawLineToTowed(item->x, item->z,
            item->towingPoint->x, item->towingPoint->z);   
    }
}
void MainWindow::drawStreamer(Streamer *item)
{
    DrawingAreaTopView->drawStreamer(item->x, item->y,
        item->getChan(item->getChanCount())->x,
        item->getChan(item->getChanCount())->y, item->getChanCount());
    DrawingAreaSideView->drawStreamer(item->x, item->z,
        item->getChan(item->getChanCount())->x,
        item->getChan(item->getChanCount())->z,item->getChanCount());
}

void MainWindow::on_TopViewRB_clicked()
{
    DrawingAreaTopView->show();
    DrawingAreaSideView->hide();
}


void MainWindow::on_SideViewRB_clicked()
{
    DrawingAreaTopView->hide();
    DrawingAreaSideView->show();
}



void MainWindow::on_ComboBoxItemType_activated(int index)
{
    switch(index) {
    case 0:
        ui->ItemNameLineEdit->setText("");
        ui->NeedConnectionCB->setChecked(false);
        ui->NeedConnectionCB->setEnabled(true);
        break;
    case 1:
        ui->ItemNameLineEdit->setText("Source");
        ui->NeedConnectionCB->setEnabled(true);
        break;
    case 2:
        ui->ItemNameLineEdit->setText("Buoy");
        ui->NeedConnectionCB->setChecked(false);
        ui->NeedConnectionCB->setEnabled(true);
        break;
    case 3:
        ui->ItemNameLineEdit->setText("Streamer");
        ui->NeedConnectionCB->setChecked(false);
        ui->NeedConnectionCB->setEnabled(false);
        break;
    default:
        ui->ItemNameLineEdit->setText("");
        ui->NeedConnectionCB->setEnabled(true);
        break;
    }
}


void MainWindow::on_doubleSpinBox_valueChanged(double boardHeight) //изменяет высоты борта в полях буксируемых объектов
{
    emit coordinator->boardDepthChanged(boardHeight);
}




void MainWindow::on_pushButton_clicked()
{
    static int timerRunsFlag = 0;
    if (!timerRunsFlag) {
        timerRunsFlag = 1;
        coordinator->wireFixedItems();
        p190Creator->createP190File();
        MyTimer->start(200);
        // qDebug() << "start";
    } else {
        timerRunsFlag = 0;
        MyTimer->stop();
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    for (FixedItem* it: Vault->ItemsVault) {
        if (it->hasConnection) {
            it->connection->reconnect();
        }
    }
}


