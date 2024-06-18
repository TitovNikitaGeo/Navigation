#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mygraphicview.h"
#include "sidegraphicview.h"
#include "fabric.h"
#include "itemsstorage.h"
#include "connection_creator.h"

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


    FixedItem* createFixedItem(Fabric::FixedItemInfo NewItemInfo);
    FixedItem* createTowedItem(Fabric::TowedItemInfo NewItemInfo);

    QString GetNewDeviceName(QString name);

private:
    void drawLineToTowed(TowedItem* item);
    void addItemToObjectsList(FixedItem* newItem);
    void setUpObjectsTable();


    ///TODO
    void CreateShipItem();
    ///TODO

    Ui::MainWindow *ui;

    MyGraphicView *DrawingAreaTopView;
    SideGraphicView *DrawingAreaSideView;
    Fabric* MyFabric;
    ItemsStorage* Vault;
    QTableWidget* tableWithItems;

    QTimer* MyTimer;


    int NumberOfRowsInTable = 0;


private slots:
    void on_DeleteItemPushButton_clicked();
    void on_AddItemtPushButton_clicked();

    void on_RBFixed_clicked();
    void on_RBTowed_clicked();
    void timeToCollectData();

    void on_TopViewRB_clicked();
    void on_SideViewRB_clicked();
    void on_pushButton_clicked(bool checked);
};
#endif // MAINWINDOW_H
