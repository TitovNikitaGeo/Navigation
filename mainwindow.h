#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mygraphicview.h"
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


private slots:
    void on_DeleteItemPushButton_clicked();
    void on_AddItemtPushButton_clicked();

    void on_RBFixed_clicked();
    void on_RBTowed_clicked();


private:
    void drawLineToTowed(TowedItem* item);
    void addItemToObjectsList(FixedItem* newItem);
    void setUpObjectsTable();


    Ui::MainWindow *ui;

    MyGraphicView *DrawingArea;
    Fabric* MyFabric;
    ItemsStorage* Vault;
    QTableWidget* tableWithItems;

    QTimer* MyTimer;


    int NumberOfRowsInTable = 0;


private slots:
    void timeToCollectData();

    // QHash<QString, FixedItem*> ItemsHashMap;

};
#endif // MAINWINDOW_H