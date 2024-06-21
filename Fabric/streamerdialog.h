#ifndef STREAMERDIALOG_H
#define STREAMERDIALOG_H

#include <QDialogButtonBox>
#include <QDialog>
#include <QObject>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

// #include "itemsstorage.h"


class StreamerDialog : public QDialog
{
    Q_OBJECT
public:
    StreamerDialog(QWidget *parent = nullptr);
    // StreamerDialog(ItemsStorage* Vault);

    QString retrunValue = "";
    uint numOfChannels = 0;
    QVector<float> chans = {};


private:
    QPushButton *finishButton;
    QTabWidget *tabWidget;
    QWidget *singleChannelTab;
    QWidget *multiChannelTab;

    QComboBox *singleChannelComboBox;
    QComboBox *multiChannelComboBoxChannels;
    QComboBox *multiChannelComboBoxDistance;
private slots:
    void onFinishButtonClicked();
};

#endif // STREAMERDIALOG_H
