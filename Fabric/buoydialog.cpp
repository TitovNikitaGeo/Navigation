#include "buoydialog.h"

BuoyDialog::BuoyDialog(ItemsStorage* vault, bool needConnection) : MyVault(vault), needConnection(needConnection) {
    setWindowTitle("BuoyDialog");
    ///если без соединения, то к косе не привязан, т.е. он не концевой
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *comboBoxLayout = new QHBoxLayout();

    if (needConnection){
        QLabel *comboBoxLabel = new QLabel("Streamer Items:");
        comboBox = new QComboBox();
        comboBoxLayout->addWidget(comboBoxLabel);
        comboBoxLayout->addWidget(comboBox);
        mainLayout->addLayout(comboBoxLayout);
        for (const auto &item : vault->ItemsVault) {
            if (QString(item->metaObject()->className()) ==  "Streamer") {
                comboBox->addItem(item->name);
            }
        }
    }

    QHBoxLayout *spinBoxLayout = new QHBoxLayout();
    QLabel *spinBoxLabel1 = new QLabel("Anthena Height:");
    spinBox1 = new QDoubleSpinBox();
    spinBox1->setRange(0.5, 2.5);
    spinBox1->setSingleStep(0.1);
    spinBox1->setValue(1.25);

    QLabel *spinBoxLabel2 = new QLabel("Towing Depth:");
    spinBox2 = new QDoubleSpinBox();
    spinBox2->setRange(0.0, 5.0);
    spinBox2->setSingleStep(0.1);
    spinBox2->setValue(0.15);

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
    if (needConnection) {
        if (comboBox->count() == 0) {
            reject();
        } else {
            selectedStreamerName = comboBox->currentText();
            selectedStreamer = qobject_cast<Streamer*>(MyVault->getItem(selectedStreamerName));
        }
        qDebug() << MyVault->getItem(selectedStreamerName)->name << "buoyDialog";
    }
    AnthenaHeight = spinBox1->value();
    TowingDepth = spinBox2->value();
    accept();
}
