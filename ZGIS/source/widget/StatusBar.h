#pragma once
#include <QtWidgets/qstatusbar.h>

class StatusBar:public QStatusBar
{
public:
	StatusBar(QWidget* parent = Q_NULLPTR);
	~StatusBar();

private:

};