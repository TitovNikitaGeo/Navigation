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

#include "nmeaparser.h"
#include "fixeditem.h"



class P190_creator : public QObject
{
    Q_OBJECT
public:

    explicit P190_creator(QObject *parent = nullptr);

    ///creating P190 data
    QStringList createRecieverData();
    QStringList createMainInfoBlock();
    QStringList createHeader();

    void createP190File(); /// create file, write header
    void writeP190Block(); /// writing streamer and reciever blocks
    void writeToFile(QStringList);

    void setFileName(QString fileName);
    void updateItemsList(FixedItem* newItem);
private:

    QStringList currentStreamerData;


    QString fileName;
    QFile* outputFile;
    QTextStream* outputStream;

    QVector<FixedItem*> items;


signals:

};

#endif // P190_CREATOR_H
