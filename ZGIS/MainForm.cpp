#include "MainForm.h"

ZGIS::ZGIS(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.openGLWidget = new OpenGLWidget(ui.centralWidget, "hello");
    ui.openGLWidget->show();
}
