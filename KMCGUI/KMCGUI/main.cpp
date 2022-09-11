#include "KMCGUI.h"
#include <QtWidgets/QApplication>
#include "kmc_runner.h"
#include <QLABEL>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KMCGUI w;
    w.show();
    return a.exec();
}
