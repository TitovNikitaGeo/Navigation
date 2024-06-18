/*#ifndef CONNECTION_CREATOR_H
#define CONNECTION_CREATOR_H

#include <QMainWindow>
#include <QVector>
#include <QFileDialog>
#include <QStandardPaths>

#include "connection_com.h"
#include "connection_net.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class Connection_creator;
}
QT_END_NAMESPACE

class Connection_creator : public QMainWindow
{
    Q_OBJECT

public:
    Connection_creator(QWidget *parent = nullptr);
    ~Connection_creator();


    Connection* createConnection();


private slots:
    void on_pushButton_clicked();

private:
    Ui::Connection_creator *ui;
    QVector<Connection*> Connection_vector;
    QVector<QString> Filenames_vector;

};
#endif // CONNECTION_CREATOR_H
 */

#ifndef CONNECTION_CREATOR_H
#define CONNECTION_CREATOR_H

#include <QDialog>
#include <QVector>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSerialPortInfo>

#include "connection_com.h"
#include "connection_net.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Connection_creator;
}
QT_END_NAMESPACE

class Connection_creator : public QDialog
{
    Q_OBJECT

public:
    Connection_creator(QWidget *parent = nullptr);
    ~Connection_creator();
    Connection* createConnection();
    Connection* getLastConnection();


private slots:
    void on_pushButton_clicked();

signals:
    void connectionCreated();

private:
    Connection* lastConnection;

    Ui::Connection_creator *ui;
    QVector<Connection*> Connection_vector;
    QVector<QString> Filenames_vector;
};

#endif // CONNECTION_CREATOR_H
