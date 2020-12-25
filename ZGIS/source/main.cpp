#include <QtWidgets/QApplication>
#include "widget/MainForm.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZGIS w;
    w.show();
    return a.exec();
}
