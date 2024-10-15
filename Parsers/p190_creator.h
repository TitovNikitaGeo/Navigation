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
#include <QCoreApplication>
#include <QRegularExpression>

#include "itemsstorage.h"
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
    QStringList createMainInfoBlock(int ffid);
    QString createMainRow(FixedItem* item, int pointNumber, int tailBuoy);

    void createP190File(); /// create file, write header
    void createP190File(int dayNumber);

    QStringList createHeader();
    QStringList createHeader(int dayNumber);

    void writeP190Block(); /// writing streamer and reciever blocks
    void writeToFile(QStringList);

    void setItemStoragePtr(ItemsStorage* Vault);
    void setLineName(const QString &newLineName);
    QString createFileName();
    QString convertCoordinates(const QString &input);

    QString createMainRow__new(FixedItem* item, int pointNumber, int tailBuoy);
    void setPath(const QDir &newPath);

    void setFFID(int newFFID);

    ItemsStorage *getMyVault() const;
    void setMyVault(ItemsStorage *newMyVault);

    void setDayOfSurvey(int newDayOfSurvey);

private:
    int currentBuoyNumber;
    int FFID;

    QDateTime dummyTimeForItemsWithNoTime;

    QDateTime curDateTime;
    int dayOfSurvey = -1;

    QString fileName;
    QFile* outputFile;
    QTextStream* outputStream;
    ItemsStorage* MyVault;

    QDir path;

    QString lineName = "AA10AA0707077";
    QChar tailBuoy; //ну и костыль. руки бы оторвать разрабу
    // QString floatToQString(float value, int totalLength, int decimalPlaces);


    bool headerIsWritten = false;

    QString replaceDates(QString& input);
signals:

};

#endif // P190_CREATOR_H
