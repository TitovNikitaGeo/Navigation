#include "mainwindow.h"
#include "test_postprocessing.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Data/icon.ico"));
    testPostprocessing test;
    return 666;


    MainWindow w;
    w.show();

    return a.exec();
}
