#ifndef STREAMERDIALOG_H
#define STREAMERDIALOG_H

#include <QDialogButtonBox>
#include <QDialog>
#include <QObject>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMessageBox>


// #include "itemsstorage.h"


class StreamerDialog : public QDialog
{
    Q_OBJECT
public:
    StreamerDialog(QWidget *parent = nullptr);
    // StreamerDialog(ItemsStorage* Vault);

    QString retrunValue = "";
    uint numOfChannels = 0;
    QVector<double> chans = {};
    double elasticSectionLength = 12.5;
    double totalLength;

    double leadingDamperLength;
    double endDamperLength;
    void saveToCSV();

    QString dChString = "";


private:
    QDoubleSpinBox *elasticSectionLengthSpinBox;
    QPushButton *finishButton;
    QPushButton* loadCSVButton;
    QPushButton* saveCSVButton;
    QTabWidget *tabWidget;
    QWidget *singleChannelTab;
    QWidget *multiChannelTab;

    QComboBox *singleChannelComboBox;
    QComboBox *multiChannelComboBoxChannels;
    QComboBox *multiChannelComboBoxDistance;

    void createChansValues();
private slots:
    void loadCSVFile();
    void readCSV(const QString& filePath);
    void onFinishButtonClicked();
};

#endif // STREAMERDIALOG_H
