#include "streamerdialog.h"

StreamerDialog::StreamerDialog(QWidget* parent) {
    setFixedSize(250, 192);
    tabWidget = new QTabWidget(this);

    // Вкладка Одноканальная
    singleChannelTab = new QWidget(this);
    singleChannelComboBox = new QComboBox(singleChannelTab);
    singleChannelComboBox->addItems({
        "HRStreamer-1×8",
        "HRStreamer-1×16",
        "HRStreamer-1×24",
        "HRStreamer-1×48",
        "HRStreamer-1×96"
    });

    QVBoxLayout *singleChannelLayout = new QVBoxLayout(singleChannelTab);
    singleChannelLayout->addWidget(singleChannelComboBox);
    singleChannelLayout->addStretch(1);  // Отступ снизу
    singleChannelTab->setLayout(singleChannelLayout);

    // Вкладка Многоканальная
    multiChannelTab = new QWidget(this);

    QLabel *channelsLabel = new QLabel("Количество каналов", multiChannelTab);
    multiChannelComboBoxChannels = new QComboBox(multiChannelTab);
    multiChannelComboBoxChannels->addItems({"24", "48"});

    QLabel *distanceLabel = new QLabel("Расстояние между каналами", multiChannelTab);
    multiChannelComboBoxDistance = new QComboBox(multiChannelTab);
    multiChannelComboBoxDistance->addItems({"1", "2", "3.125"});

    QVBoxLayout *multiChannelLayout = new QVBoxLayout(multiChannelTab);
    multiChannelLayout->addWidget(distanceLabel);
    multiChannelLayout->addWidget(multiChannelComboBoxDistance);
    multiChannelLayout->addWidget(channelsLabel);
    multiChannelLayout->addWidget(multiChannelComboBoxChannels);
    multiChannelLayout->addStretch(1);  // Отступ снизу
    multiChannelTab->setLayout(multiChannelLayout);

    // Дополнительный вариант для многоканальной вкладки
    connect(multiChannelComboBoxChannels, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        multiChannelComboBoxDistance->clear();
        if (index == 1) { // 48 каналов выбрано
            multiChannelComboBoxDistance->addItems({"1", "2", "1&2", "3.125"});
        } else { // 24 канала выбрано
            multiChannelComboBoxDistance->addItems({"1", "2", "3.125"});
        }
    });

    tabWidget->addTab(singleChannelTab, "Одноканальная");
    tabWidget->addTab(multiChannelTab, "Многоканальная");

    finishButton = new QPushButton("Finish", this);
    connect(finishButton, &QPushButton::clicked, this, &StreamerDialog::onFinishButtonClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(finishButton);
    mainLayout->setAlignment(finishButton, Qt::AlignRight); // Выравнивание кнопки вправо
    setLayout(mainLayout);
}

void StreamerDialog::onFinishButtonClicked()
{
    chans.clear();//на случай если несколько раз окно вызываем
    if (tabWidget->currentIndex() == 1) { //multichannel streamer
        bool ok;
        numOfChannels = multiChannelComboBoxChannels->currentText().toUInt();
        float dCh = multiChannelComboBoxDistance->currentText().toFloat(&ok);
        if (ok) {
            for (uint i = 0; i < numOfChannels; i++) {
                chans.append(i*dCh);
            }
        } else { //1&2
            dCh = 1;
            for (uint i = 0; i < 24; i++) {
                chans.append(i*dCh);
            }
            dCh = 2;
            for (uint i = 0; i < 24; i++) {
                chans.append(chans[23]+i*dCh);
            }
        }
    } else{ //HRStreamer-1
        float dCh = 0.2;
        switch(singleChannelComboBox->currentIndex()){
        case 0:
            numOfChannels = 2;
            for (uint i = 0; i < numOfChannels; i++) {
                chans.append(i*dCh);
            }
            qDebug() << "1x8";
        case 1:
            numOfChannels = 3;
            qDebug() << "1x16";
        case 2:
            numOfChannels = 4;
            qDebug() << "1x24";
        case 3:
            numOfChannels = 5;
            qDebug() << "1x48";
        case 4:
            numOfChannels = 6;
            qDebug() << "1x96";
        }
    }

    accept();
}

