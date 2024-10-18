#include "postprocessorview.h"

PostProcessorView::PostProcessorView(QWidget *parent)
    : QWidget{parent}
{
    setupUI();
}


void PostProcessorView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Блок для выбора схемы
    QHBoxLayout* schemeLayout = new QHBoxLayout();
    chooseSchemeButton = new QPushButton("Choose scheme", this);
    schemeLineEdit = new QLineEdit(this);
    schemeLineEdit->setReadOnly(true);
    schemeLayout->addWidget(chooseSchemeButton);
    schemeLayout->addWidget(schemeLineEdit);
    mainLayout->addLayout(schemeLayout);

    // Подключение кнопки к функции выбора файла
    connect(chooseSchemeButton, &QPushButton::clicked, this, &PostProcessorView::chooseScheme);

    // Кнопка RUN
    runButton = new QPushButton("RUN", this);
    runButton->setEnabled(false);  // Активируется только после получения всех параметров.
    mainLayout->addWidget(runButton);

    // Прогресс-бар (по умолчанию скрыт)
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);

    // Выбор FFID source
    QHBoxLayout* ffidLayout = new QHBoxLayout();
    chooseFfidButton = new QPushButton("Choose FFID source", this);
    ffidLineEdit = new QLineEdit(this);
    ffidLineEdit->setReadOnly(true);
    segyRadioButton = new QRadioButton("SEGY", this);
    txtRadioButton = new QRadioButton("TXT", this);
    txtRadioButton->setChecked(true);  // По умолчанию выбран TXT
    ffidLayout->addWidget(chooseFfidButton);
    ffidLayout->addWidget(ffidLineEdit);
    ffidLayout->addWidget(segyRadioButton);
    ffidLayout->addWidget(txtRadioButton);
    mainLayout->addLayout(ffidLayout);

    // Подключение выбора FFID
    connect(chooseFfidButton, &QPushButton::clicked, this, &PostProcessorView::chooseFfidSource);

    // Логика проверки заполненности всех полей для активации RUN
    connect(schemeLineEdit, &QLineEdit::textChanged, this, &PostProcessorView::checkReadyToRun);
    connect(ffidLineEdit, &QLineEdit::textChanged, this, &PostProcessorView::checkReadyToRun);

}

void PostProcessorView::sendValuesToPostProc()
{

    QString fileJsonName;
    QVector<QString> nmeaNames;
    QVector<QString> ppkNames;
    QString ffidTimeSourceTxt = "";
    QString ffidTimeSourceDir = "";

    QFile* jsonSchemeFile = new QFile(fileJsonName);

    QVector<QFile*> nmeaFiles;
    for (auto i: nmeaNames) {
        QFile* nmeaFile = new QFile(i);
        nmeaFiles.append(nmeaFile);

    }

    QVector<QFile*> ppkFiles;
    QFile ffidTimeSourceTxtFile;
    QFile ffidTimeSourceDirFile;
}


void PostProcessorView::chooseScheme() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choose scheme", QString(), "*.json");
    if (!fileName.isEmpty()) {
        schemeLineEdit->setText(fileName);
        // postProc; // Заглушка для парсинга, заполняем items и itemsWithConnection.
        createConnectionButtons();
    }
}

void PostProcessorView::createConnectionButtons() {
    // Очистка существующих кнопок

    QVector<QPair<QString, QString>> pairItemFile;

    for (auto* btn : nmeaPpkButtons) {
        btn->deleteLater();
    }
    for (auto* lineEdit : nmeaPpkLineEdits) {
        lineEdit->deleteLater();
    }
    nmeaPpkButtons.clear();
    nmeaPpkLineEdits.clear();

    // Создание кнопок и полей по количеству itemsWithConnection
    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    QVector<QString> namesOfItemsWithConnection;
    for (auto i:postProc.Vault.ItemsVault){
        if (i->hasConnection) {
            namesOfItemsWithConnection.append(i->name);
        }
    }
    for (int i = 0; i < namesOfItemsWithConnection.size(); ++i) {
        QPushButton* nmeaPpkButton =
            new QPushButton(QString("Choose file %1").arg(namesOfItemsWithConnection[i]), this);
        QLineEdit* nmeaPpkLineEdit = new QLineEdit(this);
        nmeaPpkLineEdit->setReadOnly(true);

        buttonsLayout->addWidget(nmeaPpkButton);
        buttonsLayout->addWidget(nmeaPpkLineEdit);

        nmeaPpkButtons.append(nmeaPpkButton);
        nmeaPpkLineEdits.append(nmeaPpkLineEdit);

        connect(nmeaPpkButton, &QPushButton::clicked, this, [this, nmeaPpkLineEdit]() {
            QString fileName = QFileDialog::getOpenFileName(this, "Choose NMEA or PPK file", QString(), "*.nmea *.ppk");
            if (!fileName.isEmpty()) {
                nmeaPpkLineEdit->setText(fileName);
                checkReadyToRun();
            }
        });
        QPair<QString,QString> pair(namesOfItemsWithConnection[i], nmeaPpkLineEdit->text());
        pairItemFile.append(pair);
    }
    layout()->addItem(buttonsLayout);
}

void PostProcessorView::chooseFfidSource() {
    if (segyRadioButton->isChecked()) {
        QString dirName = QFileDialog::getExistingDirectory(this, "Choose SEGY directory");
        if (!dirName.isEmpty()) {
            ffidLineEdit->setText(dirName);
        }
    } else if (txtRadioButton->isChecked()) {
        QString fileName = QFileDialog::getOpenFileName(this, "Choose TXT file", QString(), "*.txt");
        if (!fileName.isEmpty()) {
            ffidLineEdit->setText(fileName);
        }
    }
}

void PostProcessorView::checkReadyToRun() {
    // Проверяем, что все необходимые QLineEdit заполнены
    bool allFieldsFilled = !schemeLineEdit->text().isEmpty() && !ffidLineEdit->text().isEmpty();
    for (auto* lineEdit : nmeaPpkLineEdits) {
        if (lineEdit->text().isEmpty()) {
            allFieldsFilled = false;
            break;
        }
    }
    runButton->setEnabled(allFieldsFilled);
}


void PostProcessorView::onRunButtonClicked() {
    // Инициализация прогресса
    progressBar->setVisible(true);
    float progressValue = 0;
    progressBar->setValue(progressValue);
}
