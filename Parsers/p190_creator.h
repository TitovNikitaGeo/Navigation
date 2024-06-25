#ifndef P190_CREATOR_H
#define P190_CREATOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QVector>

#include "itemsstorage.h"
#include "streamer.h"
// #include "functions.h"



class P190_creator : public QObject
{
    Q_OBJECT
public:

    explicit P190_creator(QObject *parent = nullptr);

    ///creating P190 data
    void extracted(QStringList &res);
    QStringList createStreamerData();
    QStringList createMainInfoBlock();
    QStringList createHeader();

    void createP190File(); /// create file, write header
    void writeP190Block(); /// writing streamer and reciever blocks
    void writeToFile(QStringList);

    void setFileName(QString fileName);

    void setItemStoragePtr(ItemsStorage* Vault);
private:

    QStringList currentStreamerData;


    QString fileName;
    QFile* outputFile;
    QTextStream* outputStream;
    ItemsStorage* MyVault;

    QString lineName = "AA10AA070707";
    uint pointNumber = 1000;

signals:

};

#endif // P190_CREATOR_H
