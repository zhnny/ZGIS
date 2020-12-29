#pragma once
#include "GL/glew.h"
#include <QtWidgets/qopenglwidget.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <gdal/ogrsf_frmts.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <mapbox/earcut.hpp>
#include "model/GeoMap.h"
#include "model/GeoBase.hpp"
#include "whatisthisdialog.h"
#include "opengl/openglfeaturedescriptor.h"
#include "opengl/env.h"
#include "opengl/appevent.h"
#include "opengl/vertexbufferlayout.h"


class OpenGLWidget:public QOpenGLWidget
{
	Q_OBJECT
public:
	OpenGLWidget(QWidget* parent = Q_NULLPTR);
	~OpenGLWidget();

signals:
	/// <summary>
	/// 更新status bar的地理坐标
	/// </summary>
	void signalUpadteCoord(double geoX, double geoY);

public slots:
	void onSendMapToGPU(bool update = true);
	void onSendDataSourceToGPU(OGRDataSource* dataSource,bool update=true);
	void onSendLayerToGPU(OGRLayer* layer,bool update=true);
	void onSendFeatureLayerToGPU(OGRLayer* layer,bool update=true);
	void onSendFeatureToGPU(OGRFeature* feature);
	void onZoomToLayer(OGRLayer* layer);
	void onZoomToMap();

private:
	OpenglFeatureDescriptor* sendPointToGPU(OGRPoint* point, float r, float g, float b);
	OpenglFeatureDescriptor* sendMultiPointToGPU(OGRMultiPoint* mutliPoint, float r, float g, float b);
	OpenglFeatureDescriptor* sendLineStringToGPU(OGRLineString* lineString, float r, float g, float b);
	OpenglFeatureDescriptor* sendMultiLineStringToGPU(OGRMultiLineString* multiLineString, float r, float g, float b);
	OpenglFeatureDescriptor* sendPolygonToGPU(OGRPolygon* polygon, float r, float g, float b);
	OpenglFeatureDescriptor* sendMultiPolygonToGPU(OGRMultiPolygon* multiPolygon, float r, float g, float b);


protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;

	/// <summary>
	/// event
	/// </summary>
	virtual void mousePressEvent(QMouseEvent* ev) override;
	virtual void mouseMoveEvent(QMouseEvent* ev) override;
	virtual void mouseReleaseEvent(QMouseEvent* ev) override;
	virtual void wheelEvent(QWheelEvent* ev) override;
	virtual void enterEvent(QEvent*) override;
	virtual void keyPressEvent(QKeyEvent* ev) override;

private:
	std::vector<double> point;

	GeoMap*& map;

	QPoint xy2screen(double geoX, double geoY);
	GeoRawPoint screen2xy(int screenX, int screenY);
	GeoRawPoint screen2stdxy(int screenX, int screenY);
	double getLengthInWorldSystem(int screenLength);

	void updateMVP(bool updateModel = true, bool updateView = true, bool updateProj = false);
	void setMVP();

	void clearSelected();
	void drawRectNoFill(const QPoint& startPoint, const QPoint& endPonit, float r, float g, float b, float a = 1.0f);

	void drawRectFillColor(const QPoint& startPoint, const QPoint& endPoint, float r = 0.0f, float g = 0.0f, float b = 0.0f, int lineWidth = 1);

private:
	// model
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float zoom = 1.0f;
	glm::mat4 trans;	// translation
	glm::mat4 scale;	// sacle
	glm::mat4 model;
	// view
	glm::mat4 view;
	// project
	glm::mat4 proj;

	GeoExtent adjustedMapExtent;

	bool isMouseClicked = false;
	bool isRunning = true;
	bool isRectSelecting = false;
	bool isSelected = false;
	bool isMovingFeatures = false;
	bool isModified = false;

	// What is this
	WhatIsThisDialog* whatIsThisDialog = nullptr;

	QPoint mouseLastPos;
	QPoint mouseBeginPos;   // when press left button
	QPoint mouseCurrPos;    // when move mouse
};
