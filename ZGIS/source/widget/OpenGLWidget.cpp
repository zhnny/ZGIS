#include "OpenGLWidget.h"
#include "glut.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
{
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::onSendDataSourceToGPU(OGRDataSource* dataSource)
{
	if (!dataSource)
		return;
	makeCurrent();

	OGRLayer* layer = NULL;
	for (int i = 0; i < dataSource->GetLayerCount(); i++)
	{
		layer = dataSource->GetLayer(i);
		onSendFeatureLayerToGPU(layer);
	}
}

void OpenGLWidget::onSendLayerToGPU(OGRLayer* layer)
{

}

void OpenGLWidget::onSendFeatureLayerToGPU(OGRLayer* layer)
{
	if (!layer)
		return;
	makeCurrent();

	for (int i = 0; i < layer->GetFeatureCount(); i++)
	{
		onSendFeatureToGPU(layer->GetFeature(i));
	}
}

void OpenGLWidget::onSendFeatureToGPU(OGRFeature* feature)
{
	if (!feature)
		return;
	makeCurrent();
	OGRGeometry* geometry = feature->GetGeometryRef();
	switch (geometry->getGeometryType())
	{
	case wkbPoint:
		sendPointToGPU(geometry->toPoint());
		break;

	case wkbLineString:
		sendLineStringToGPU(geometry->toLineString());
		break;

	case wkbPolygon:
		sendPolygonToGPU(geometry->toPolygon());

		break;
	}
}

void OpenGLWidget::onZoomToLayer(OGRLayer* layer)
{
}

void OpenGLWidget::onZoomToMap()
{
}

void OpenGLWidget::sendPointToGPU(OGRPoint* point)
{
}

void OpenGLWidget::sendLineStringToGPU(OGRLineString* lineString)
{
	int i = 0;
	int j = lineString->getNumPoints();
	for (i; i < lineString->getNumPoints(); ++i)
	{
		point.push_back(lineString->getX(i));
		point.push_back(lineString->getY(i));
	}
}

void OpenGLWidget::sendPolygonToGPU(OGRPolygon* polygon)
{
	int i = 0;
	int j = polygon->getNumInteriorRings();
	OGRLinearRing* linearRing = polygon->getExteriorRing();
	for (i; i < linearRing->getNumPoints(); i = i + 2)
	{
		point.push_back(linearRing->getX(i));
		point.push_back(linearRing->getY(i));
	}
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
/// <summary>
/// 使用Qt封装的OpenGL
/// </summary>
void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(-1.5, 0.0, -6.0);

	glBegin(GL_COLOR);
	glColor3f(0.0, 1.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	double p1 = 0.0;
	double p2 = 0.0;
	for (int i = 0; i < point.size(); i = i + 2)
	{
		if (i > point.size() - 4)
			break;
		p1 = point[i] / 50;
		p2 = point[i + 1.0] / 50;
		glVertex3f(p1,p2 , 0.0);
		p1 = point[i+2.0] / 50;
		p2 = point[i + 3.0] / 50;
		glVertex3f(p1, p2, 0.0);
		//除以50是随便想的，为了让坐标范围缩小到OpenGL可以显示的范围。
	}
	glEnd();

	glTranslatef(3.0, 0.0, 0.0);
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
