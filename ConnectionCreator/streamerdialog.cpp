#include "streamerdialog.h"
#include "ui_streamerdialog.h"

StreamerDialog::StreamerDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StreamerDialog)
{
    ui->setupUi(this);
}

StreamerDialog::~StreamerDialog()
{
    delete ui;
}
