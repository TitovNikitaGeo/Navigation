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
#include <QDir>
#include <QStandardPaths>
#include <QRegExp>

#include "itemsstorage.h"
#include "streamer.h"
#include "functions.h"



class P190_creator : public QObject
{
    Q_OBJECT
public:

    explicit P190_creator(QObject *parent = nullptr);
    ~P190_creator();

    ///creating P190 data
    QStringList createShotBlock();
    QStringList createStreamerBlock();
    QStringList createMainInfoBlock();
    QStringList createHeader();
    QString createMainRow(FixedItem* item, int pointNumber);

    void createP190File(); /// create file, write header
    void writeP190Block(); /// writing streamer and reciever blocks
    void writeToFile(QStringList);

    void setItemStoragePtr(ItemsStorage* Vault);
    void setLineName(const QString &newLineName);

    QString createFileName();
    QString convertCoordinates(const QString &input);
private:
    QDateTime curDateTime;
    QString fileName;
    QFile* outputFile;
    QTextStream* outputStream;
    ItemsStorage* MyVault;

    QString lineName = "AA10AA0707077";
    // QString floatToQString(float value, int totalLength, int decimalPlaces);
signals:

};

#endif // P190_CREATOR_H
