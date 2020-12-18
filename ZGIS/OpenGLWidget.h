#pragma once
#ifndef OPENGLWIDGET_H

#include <qopenglwidget.h>
#include <gl/GLU.h>
#include <QKeyEvent>

class OpenGLWidget :public QOpenGLWidget
{
	Q_OBJECT

public:

	OpenGLWidget(QWidget* parent = 0, const char* name = 0, bool fs = false);
	~OpenGLWidget();

protected:

	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent* e) override;

protected:

	bool fullscreen;
};

#endif // !OPENGLWIDGET_H

