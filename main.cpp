#include "mainwindow.h"
#include "test_postprocessing.h"
#include "testitemsloading.h"
#include "testppkparser.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Data/icon.ico"));
    // testPostprocessing test;
    // return 444;

    // testItemsLoading test1;
    // testPpkParser test2;
    // return 666;
    MainWindow w;
    w.show();

    return a.exec();
}
