#include "postprocessorview.h"

PostProcessorView::PostProcessorView(QWidget *parent)
    : QWidget{parent}
{
    setupUI();
}

PostProcessorView *PostProcessorView::getInstance()
{
    static PostProcessorView* instance = nullptr;  // Указатель на единственный экземпляр

    if (!instance) {
        instance = new PostProcessorView();
    }
    instance->raise();  // Поднять окно на передний план
    instance->activateWindow();  // Активировать окно
    return instance;
}


void PostProcessorView::setupUI() {
    mainLayout = new QVBoxLayout(this);

    // Кнопка RUN
    runButton = new QPushButton("RUN", this);
    runButton->setEnabled(false);  // Активируется только после получения всех параметров.
    mainLayout->addWidget(runButton);

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


    // Прогресс-бар (по умолчанию скрыт)
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);
    postProc.fuckingShit = progressBar;

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
    connect(runButton, &QPushButton::clicked, this, &PostProcessorView::onRunButtonClicked);
}

void PostProcessorView::sendValuesToPostProc() //передача параметров из view в model (контроллер идет нахуй) UPD нет, мы оба
{
    QVector<QString> nmeaNames;
    QVector<QString> ppkNames;
    QString ffidTimeSourceTxt = "";
    QString ffidTimeSourceDir = "";


    for (auto i: nmeaNames) {
        QFile* file = new QFile(i);
        file->open(QIODevice::ReadOnly);
        postProc.nmeaFiles.append(file);
    }
    for (auto i:ppkNames) {
        QFile* file = new QFile(i);
        file->open(QIODevice::ReadOnly);
        postProc.ppkFiles.append(file);
    }
    if (ffidLineEdit->text().endsWith(".txt")) {
        ffidTimeSourceTxt = ffidLineEdit->text();
        // qDebug() <<ffidTimeSourceTxt <<__FUNCTION__;
    } else {
        ffidTimeSourceDir = ffidLineEdit->text();
        // qDebug() <<ffidTimeSourceDir <<__FUNCTION__;;
    }

    // pairItemFile.append(pair);
    // qDebug() << pair.first << pair.second <<__FUNCTION__ << nmeaPpkLineEdit->text();

    for (int i = 0; i < nmeaPpkLineEdits.size(); i++) {
        QString itemName = nmeaPpkButtons[i]->text();
        itemName = itemName.mid(itemName.indexOf("\"")).remove("\""); //вах какой костыль (ну а по-другому особо никак)

        QPair<QString,QString> pairNames(nmeaPpkLineEdits[i]->text(), itemName);
        // qDebug() << pair<<__FUNCTION__;
        postProc.pairItemFile.append(pairNames);
    }


    if (!ffidTimeSourceTxt.isEmpty()) {
        postProc.ffidTimeSourceTxtFile = ffidTimeSourceTxt;
    } else if (!ffidTimeSourceDir.isEmpty()) {
        postProc.ffidTimeSourceDir = ffidTimeSourceDir;
    } else {
        qWarning() << __FUNCTION__;
    }
}


void PostProcessorView::chooseScheme() { //выбор съемы и парсинг json файла
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(this, "Choose scheme", documentsPath + "/Ship_logs/json", "*.json");
    if (!fileName.isEmpty()) {
        schemeLineEdit->setText(fileName);
        postProc.jsonSchemeFile = new QFile(fileName);
        postProc.items = ItemsLoader::readFromJSON(postProc.jsonSchemeFile);
        postProc.MyVault = new ItemsStorage(); //не особо нужно

        //заполняем поля postproc (не помню зачем, но хуже не будет)
        postProc.vectorWithCon.clear();
        postProc.vectorNoCon.clear();

        for (FixedItem* i:postProc.items) {
            postProc.MyVault->SaveItem(i);
            if (i->hasConnection) {
                postProc.vectorWithCon.append(i);
                postProc.itemsWithConnection++;
                // qDebug() << __FUNCTION__ << i->name;
            } else {
                postProc.vectorNoCon.append(i);
                // qDebug() << __FUNCTION__ << i->name;
            }
        }

        createConnectionButtons();
    }
}

void PostProcessorView::createConnectionButtons() {

    // Очистка существующих кнопок
    for (auto* btn : nmeaPpkButtons) {
        btn->deleteLater();
    }
    for (auto* lineEdit : nmeaPpkLineEdits) {
        lineEdit->deleteLater();
    }
    nmeaPpkButtons.clear();
    nmeaPpkLineEdits.clear();

    // Создание кнопок и полей по количеству itemsWithConnection
    QVBoxLayout* buttonsLayout = new QVBoxLayout();


    QVector<QString> namesOfItemsWithConnection;
    for (auto i: postProc.vectorWithCon){
        if (i->hasConnection) {
            namesOfItemsWithConnection.append(i->name);
            // qDebug() << i->name << __FUNCTION__;
        }
    }
    for (int i = 0; i < namesOfItemsWithConnection.size(); ++i) {
        QPushButton* nmeaPpkButton =
            new QPushButton(QString("Choose file \"%1\"").arg(namesOfItemsWithConnection[i]), this);
        //имя объекта берется из текста на кнопке, потому осторожнее с форматированием (см PostProcessorView::sendValuesToPostProc)
        QLineEdit* nmeaPpkLineEdit = new QLineEdit(this);
        nmeaPpkLineEdit->setReadOnly(true);

        buttonsLayout->addWidget(nmeaPpkButton);
        buttonsLayout->addWidget(nmeaPpkLineEdit);

        nmeaPpkButtons.append(nmeaPpkButton);
        nmeaPpkLineEdits.append(nmeaPpkLineEdit);


        connect(nmeaPpkButton, &QPushButton::clicked, this, [this, nmeaPpkLineEdit]() {
            QString navFilesPath = QDir("C:/Users/sabrahar/Desktop/FINAL/PPK GNSS data/test").absolutePath(); ///TODO: потом заменить на норм место
            QString fileName = QFileDialog::getOpenFileName(this, "Choose NMEA or PPK file", navFilesPath, "*.nmea *.ppk");
            if (!fileName.isEmpty()) {
                nmeaPpkLineEdit->setText(fileName);
                checkReadyToRun();
            }
        });
    }

    mainLayout->addLayout(buttonsLayout, 1);

}

void PostProcessorView::chooseFfidSource() {
    if (segyRadioButton->isChecked()) {
        QString dirName = QFileDialog::getExistingDirectory(this, "Choose SEGY directory", QString("C:/Users/sabrahar/Desktop/FINAL/"));
        if (!dirName.isEmpty()) {
            ffidLineEdit->setText(dirName);
        }
    } else if (txtRadioButton->isChecked()) {
        QString fileName = QFileDialog::getOpenFileName(this, "Choose TXT file", QString("C:/Users/sabrahar/Desktop/FINAL/"), "*.txt");
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

    // Инициализация прогресс-бара
    progressBar->setValue(1);
    progressBar->setVisible(true);
    sendValuesToPostProc();
    postProc.mainProcess();
    progressBar->setValue(0);
    hide();
    // this->close();
    // delete this;

    //мутим ffid-время-дата
    // QVector<SegYReader::Pair> pairs;
    // if (ffidLineEdit->text().endsWith(".txt")) {
    //     postProc.pairs = postProc.readFileAndGeneratePairs(postProc.ffidTimeSourceTxtFile->fileName());
    // } else {
    //     postProc.setSegyStorage(postProc.segyStorage);
    //     postProc.getDataFromSegy();
    // }
    // for (auto i: postProc.pairs) {
    //     qDebug() << i.ffid << i.time << i.julianDay;
    // }

}








