#include "buoydialog.h"

BuoyDialog::BuoyDialog(ItemsStorage* vault) : MyVault(vault) {
    setWindowTitle("BuoyDialog");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *comboBoxLayout = new QHBoxLayout();
    QLabel *comboBoxLabel = new QLabel("Streamer Items:");
    comboBox = new QComboBox();
    comboBoxLayout->addWidget(comboBoxLabel);
    comboBoxLayout->addWidget(comboBox);
    mainLayout->addLayout(comboBoxLayout);

    for (const auto &item : vault->ItemsVault) {
        if (item->name == "Streamer") {
            comboBox->addItem(item->name);
        }
    }

    QHBoxLayout *spinBoxLayout = new QHBoxLayout();
    QLabel *spinBoxLabel1 = new QLabel("Anthena Height:");
    spinBox1 = new QDoubleSpinBox();
    spinBox1->setRange(0.5, 2.5);
    spinBox1->setSingleStep(0.1);
    spinBox1->setValue(1);

    QLabel *spinBoxLabel2 = new QLabel("Towing Depth:");
    spinBox2 = new QDoubleSpinBox();
    spinBox2->setRange(0.0, 5.0);
    spinBox2->setSingleStep(0.1);
    spinBox2->setValue(1);

    spinBoxLayout->addWidget(spinBoxLabel1);
    spinBoxLayout->addWidget(spinBox1);
    spinBoxLayout->addWidget(spinBoxLabel2);
    spinBoxLayout->addWidget(spinBox2);
    mainLayout->addLayout(spinBoxLayout);

    QPushButton *setButton = new QPushButton("Set");
    connect(setButton, &QPushButton::clicked, this, &BuoyDialog::onSetButtonClicked);
    mainLayout->addWidget(setButton);

    setLayout(mainLayout);
}


void BuoyDialog::onSetButtonClicked() {
    if (comboBox->count() == 0) {
        reject();
    } else {
        selectedStreamerName = comboBox->currentText();
        if (!selectedStreamerName.isEmpty()) {
            selectedStreamer = qobject_cast<Streamer*>(MyVault->getItem(selectedStreamerName));
            qDebug() << "This Buoy connected to streamer " << selectedStreamerName;
        } else {
            qDebug() << "This Buoy doesn't connected to streamer";
            selectedStreamer = nullptr;
        }
        AnthenaHeight = spinBox1->value();
        TowingDepth = spinBox2->value();
        accept();
    }
}
