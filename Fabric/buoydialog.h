#ifndef BUOYDIALOG_H
#define BUOYDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMessageBox>

#include "itemsstorage.h"
#include "streamer.h"

class BuoyDialog : public QDialog
{
    Q_OBJECT
public:
    BuoyDialog(ItemsStorage* vault, bool needConnection);
    double AnthenaHeight;
    double TowingDepth;
    Streamer* selectedStreamer;
private:
    ItemsStorage* MyVault;
    QComboBox *comboBox;
    QDoubleSpinBox *spinBox1;
    QDoubleSpinBox *spinBox2;

    QString selectedStreamerName;

    bool needConnection;

private slots:
    void onSetButtonClicked();
};

#endif // BUOYDIALOG_H
