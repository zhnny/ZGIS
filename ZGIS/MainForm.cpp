#include "MainForm.h"

ZGIS::ZGIS(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	move(100, 20);
	connect(ui.action_shpOpen, &QAction::triggered, this, &ZGIS::GetFile);
	connect(ui.action, &QAction::triggered, this, &ZGIS::ClipLine);
	connect(ui.action_pan, &QAction::triggered, this, &ZGIS::Pan);
	connect(ui.action_zoomOut, &QAction::triggered, this, &ZGIS::ZoomOut);
	connect(ui.action_zoomIn, &QAction::triggered, this, &ZGIS::ZoomIn);
}

void ZGIS::GetFile()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		"Open Document",
		QDir::currentPath(),
		"Document files (*.shp);;All files(*.*)");
	if (!filename.isNull()) {
		mainLoop(this->frameGeometry().x() + 10,
			this->frameGeometry().y() + this->frameGeometry().height() + 30, filename.toStdString());
		//传入坐标是想将GLFW窗口置于toobar下面；后面加上的数字是看着调的
		return;
	}
	else // 用户取消选择
		return;
}

void ZGIS::Pan()
{
	setCommand("Pan");
}

void ZGIS::ZoomOut()
{
	setModel(1.25);
}

void ZGIS::ZoomIn()
{
	setModel(0.75);
}

void ZGIS::ClipLine()
{
	setCommand("ClipLine");
}
