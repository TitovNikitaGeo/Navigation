#ifndef POSTPROCESSORVIEW_H
#define POSTPROCESSORVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QRadioButton>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>

#include "postprocessor.h"

class PostProcessorView : public QWidget
{
    Q_OBJECT
public:
    explicit PostProcessorView(QWidget *parent = nullptr);

    void setupUI();
private:
    //выбранные юзверем в pprv значения для передачи в ppr
    QString fileJsonName;
    QVector<QString> nmeaNames;
    QVector<QString> ppkNames;
    QString ffidTimeSourceTxt = "";
    QString ffidTimeSourceDir = "";

    QPushButton* chooseSchemeButton;
    QLineEdit* schemeLineEdit;
    QPushButton* runButton;
    QProgressBar* progressBar;
    QVector<QPushButton*> nmeaPpkButtons;
    QVector<QLineEdit*> nmeaPpkLineEdits;
    QPushButton* chooseFfidButton;
    QLineEdit* ffidLineEdit;
    QRadioButton* segyRadioButton;
    QRadioButton* txtRadioButton;


    PostProcessor postProc;


    void sendValuesToPostProc();
signals:

private slots:
    void chooseScheme();
    void createConnectionButtons();
    void chooseFfidSource();
    void checkReadyToRun();
    void onRunButtonClicked();
};

#endif // POSTPROCESSORVIEW_H
