#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainForm.h"
#include "OpenGLWidget.h"

class ZGIS : public QMainWindow
{
    Q_OBJECT

public:
    ZGIS(QWidget *parent = Q_NULLPTR);

private:
    Ui::ZGISClass ui;
    
};
