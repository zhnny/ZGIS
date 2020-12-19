#include "MainForm.h"

ZGIS::ZGIS(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.action_shpOpen, &QAction::triggered, this, &mainLoop);
}
