#pragma once
#include "Render.h"
#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include "ui_MainForm.h"

class ZGIS : public QMainWindow
{
    Q_OBJECT

public:
    ZGIS(QWidget *parent = Q_NULLPTR);

private:
    Ui::ZGISClass ui;
    void GetFile();
};
