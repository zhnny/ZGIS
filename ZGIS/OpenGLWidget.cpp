#include "OpenGLWidget.h"
#include "includes/glut.h"


OpenGLWidget::OpenGLWidget(QWidget* parent, const char* name, bool fs)
    : QOpenGLWidget(parent)
{
    fullscreen = fs;
    setObjectName(QString::fromUtf8("openGLWidget"));
    setGeometry(QRect(0, 0, 601, 361));

    if (fullscreen)
        showFullScreen();
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(-1.5, 0.0, -6.0);

    glBegin(GL_COLOR);
    glColor3f(0.0, 1.0, 0.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);
    glEnd();

    glTranslatef(3.0, 0.0, 0.0);

    glBegin(GL_QUADS);
    glVertex3f(-1.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);
    glEnd();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
    case Qt::Key_F2:
        fullscreen = !fullscreen;
        if (fullscreen)
        {
            showFullScreen();
        }
        else
        {
            showNormal();
            setGeometry(0, 0, 640, 480);
        }
        update();
        break;
    case Qt::Key_Escape:
        close();
        update();
        break;
    case Qt::Key_Right:
        close();
        update();
        break;
    }
}
