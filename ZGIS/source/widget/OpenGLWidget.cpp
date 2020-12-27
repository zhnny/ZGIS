#include "OpenGLWidget.h"
#include <opengl/glcall.h>

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent), map(Env::map)
{
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::ClickFocus);

    connect(this, &OpenGLWidget::signalUpadteCoord,
        AppEvent::getInstance(), &AppEvent::onUpdateCoord);
    connect(AppEvent::getInstance(), &AppEvent::sigZoomToMap,
        this, &OpenGLWidget::onZoomToMap);
    connect(AppEvent::getInstance(), &AppEvent::sigZoomToLayer,
        this, &OpenGLWidget::onZoomToLayer);
    connect(AppEvent::getInstance(), &AppEvent::sigUpdateOpengl,
        this, [this] { update(); });
    connect(AppEvent::getInstance(), &AppEvent::sigSendMapToGPU,
        this, &OpenGLWidget::onSendMapToGPU);
    connect(AppEvent::getInstance(), &AppEvent::sigSendLayerToGPU,
        this, &OpenGLWidget::onSendLayerToGPU);
    connect(AppEvent::getInstance(), &AppEvent::sigSendFeatureToGPU,
        this, &OpenGLWidget::onSendFeatureToGPU);
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    isRunning = false;
}

void OpenGLWidget::onSendMapToGPU(bool update)
{

}

void OpenGLWidget::onSendDataSourceToGPU(OGRDataSource* dataSource,bool update)
{

}

void OpenGLWidget::onSendLayerToGPU(OGRLayer* layer, bool update)
{
}

void OpenGLWidget::onSendFeatureLayerToGPU(OGRLayer* layer, bool update)
{
}

void OpenGLWidget::onSendFeatureToGPU(OGRFeature* feature)
{
}

void OpenGLWidget::onZoomToLayer(OGRLayer* layer)
{
}

void OpenGLWidget::onZoomToMap()
{
}

OpenglFeatureDescriptor* OpenGLWidget::sendPointToGPU(OGRPoint* point, float r, float g, float b)
{
	return nullptr;
}

OpenglFeatureDescriptor* OpenGLWidget::sendMultiPointToGPU(OGRMultiPoint* mutliPoint, float r, float g, float b)
{
	return nullptr;
}

OpenglFeatureDescriptor* OpenGLWidget::sendLineStringToGPU(OGRLineString* lineString, float r, float g, float b)
{
	return nullptr;
}

OpenglFeatureDescriptor* OpenGLWidget::sendMultiLineStringToGPU(OGRMultiLineString* multiLineString, float r, float g, float b)
{
	return nullptr;
}

OpenglFeatureDescriptor* OpenGLWidget::sendPolygonToGPU(OGRPolygon* polygon, float r, float g, float b)
{
	return nullptr;
}

OpenglFeatureDescriptor* OpenGLWidget::sendMultiPolygonToGPU(OGRMultiPolygon* multiPolygon, float r, float g, float b)
{
	return nullptr;
}

void OpenGLWidget::initializeGL()
{
    if (glewInit() != GLEW_NO_ERROR)
    {
        return;
    }
    Env::createShaders();
    // view matrix
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    GLCall(glViewport(0, 0, w, h));
    GLCall(glMatrixMode(GL_PROJECTION));

    if (!map || map->isEmpty())
        return;
    if (h < 1)
        h = 1;

    // projection matrix
    float aspectRatio = float(w) / h;
    GeoExtent mapExtent = map->getExtent();
    if (mapExtent.aspectRatio() < aspectRatio) {
        adjustedMapExtent.minX = (mapExtent.minX - mapExtent.centerX()) * aspectRatio / mapExtent.aspectRatio() + mapExtent.centerX();
        adjustedMapExtent.maxX = (mapExtent.maxX - mapExtent.centerX()) * aspectRatio / mapExtent.aspectRatio() + mapExtent.centerX();
    }
    else {
        adjustedMapExtent.minY = (mapExtent.minY - mapExtent.centerY()) * mapExtent.aspectRatio() / aspectRatio + mapExtent.centerY();
        adjustedMapExtent.maxY = (mapExtent.maxY - mapExtent.centerY()) * mapExtent.aspectRatio() / aspectRatio + mapExtent.centerY();
    }
    proj = glm::ortho(float(adjustedMapExtent.minX), float(adjustedMapExtent.maxX),
        float(adjustedMapExtent.minY), float(adjustedMapExtent.maxY),
        -1.0f, 1.0f);

    // MVP
    setMVP();

    update();
}

void OpenGLWidget::paintGL()
{
    Env::renderer.Clear();

    if (!isRunning || !map || map->isEmpty())
        return;

    map->Draw();//Î´Íê³É12 28

    if (isRectSelecting) {
        GLCall(glUseProgram(0));
        drawRectNoFill(mouseBeginPos, mouseCurrPos, 1.0f, 0.5f, 0.0f, 5);
        drawRectFillColor(mouseBeginPos, mouseCurrPos, 1.0f, 0.5f, 0.0f, 0.5f);
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent* ev)
{
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* ev)
{
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* ev)
{
}

void OpenGLWidget::wheelEvent(QWheelEvent* ev)
{
}

void OpenGLWidget::enterEvent(QEvent*)
{
}

void OpenGLWidget::keyPressEvent(QKeyEvent* ev)
{
}

void OpenGLWidget::setMVP()
{
}

void OpenGLWidget::drawRectNoFill(const QPoint& startPoint, const QPoint& endPonit, float r, float g, float b, float a)
{
}

void OpenGLWidget::drawRectFillColor(const QPoint& startPoint, const QPoint& endPoint, float r, float g, float b, int lineWidth)
{
}
