#include "MainForm.h"

ZGIS::ZGIS(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	move(100, 20);
	connect(ui.action_shpOpen, &QAction::triggered, this, &ZGIS::GetFile);
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
		//�����������뽫GLFW��������toobar���棻������ϵ������ǿ��ŵ���
		return;
	}
	else // �û�ȡ��ѡ��
		return;
}
