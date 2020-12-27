#pragma once

#include <QtWidgets/qopenglwidget.h>
#include "gdal/ogrsf_frmts.h"
#include <vector>

class OpenGLWidget:public QOpenGLWidget
{
public:
	OpenGLWidget(QWidget* parent = Q_NULLPTR);
	~OpenGLWidget();

public slots:
	void onSendDataSourceToGPU(OGRDataSource* dataSource);
	void onSendLayerToGPU(OGRLayer* layer);
	void onSendFeatureLayerToGPU(OGRLayer* layer);
	void onSendFeatureToGPU(OGRFeature* feature);

private:
	void sendPointToGPU(OGRPoint* point);
	void sendLineStringToGPU(OGRLineString* lineString);
	void sendPolygonToGPU(OGRPolygon* polygon);

protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;

private:
	std::vector<double> point;

};
