#include "cvTools.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cvTools w;
    w.show();
    return a.exec();
}