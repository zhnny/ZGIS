#pragma once

#include <qopenglwidget.h>

class OpenGLWidget:public QOpenGLWidget
{
public:
	OpenGLWidget(QWidget* parent = Q_NULLPTR);
	~OpenGLWidget();

private:

};
