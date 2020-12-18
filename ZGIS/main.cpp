#include "MainForm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZGIS w;
    w.show();
    return a.exec();
}
