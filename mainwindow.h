#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mygraphicview.h"
#include "sidegraphicview.h"
#include "fabric.h"
#include "itemsstorage.h"
#include "itemsloader.h"
#include "coordinator.h"
#include "p190_creator.h"

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include <QTableWidget>
#include <QRegularExpression>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    struct Item;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    FixedItem* createFixedItem(FixedItemInfo FixedItemInfo);
    TowedItem* createTowedItem(TowedItemInfo TowedItemInfo);
    Streamer* createStreamerItem(StreamerInfo StreamerItemInfo);
    Buoy* createBuoyItem(BuoyInfo BuoyItemInfo);
    Source* createSourceItem(SourceInfo SourceItemInfo);

    QString GetNewDeviceName(QString name);

private:
    void drawLineToTowed(TowedItem* item);
    void drawStreamer(Streamer* item);
    void addItemToObjectsList(FixedItem* newItem);
    void setUpObjectsTable();
    void setMenuBar(QMenuBar *menuBar);

    ///TODO
    void CreateShipItem();
    ///TODO

    Ui::MainWindow *ui;

    P190_creator* p190Creator;
    MyGraphicView *DrawingAreaTopView;
    SideGraphicView *DrawingAreaSideView;
    Fabric* MyFabric;
    ItemsStorage* Vault;
    QTableWidget* tableWithItems;
    Coordinator* coordinator;
    QTimer* MyTimer;
    ItemsLoader* itemsLoader;


    int NumberOfRowsInTable = 0;


private slots:
    void on_DeleteItemPushButton_clicked();
    void on_AddItemtPushButton_clicked();

    void on_RBFixed_clicked();
    void on_RBTowed_clicked();
    void timeToCollectData();

    void on_TopViewRB_clicked();
    void on_SideViewRB_clicked();
    void on_ComboBoxItemType_activated(int index);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};
#endif // MAINWINDOW_H
