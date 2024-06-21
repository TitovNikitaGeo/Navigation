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

class BuoyDialog : public QDialog
{
    Q_OBJECT
public:
    BuoyDialog(ItemsStorage* vault);
    double AnthenaHeight;
    double TowingDepth;

private:
    ItemsStorage* MyVault;

    QComboBox *comboBox;
    QDoubleSpinBox *spinBox1;
    QDoubleSpinBox *spinBox2;

    QString selectedStreamerName;
    Streamer* selectedStreamer;


private slots:
    void onSetButtonClicked();
};

#endif // BUOYDIALOG_H
