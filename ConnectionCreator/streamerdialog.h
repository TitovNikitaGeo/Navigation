#ifndef STREAMERDIALOG_H
#define STREAMERDIALOG_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class StreamerDialog;
}

class StreamerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StreamerDialog(QWidget *parent = nullptr);
    ~StreamerDialog();

private:
    Ui::StreamerDialog *ui;
};

#endif // STREAMERDIALOG_H
