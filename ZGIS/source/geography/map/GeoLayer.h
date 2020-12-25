#pragma once
#include <QtCore/qstringlist.h>
#include <vector>
#include "GeoFeatureLayerProperty.h"

enum LayerType
{
	RasterLayer = 0,
	FeatureLayer,
	Undefined
};

class GeoLayer
{
public:
	GeoLayer() {};
	virtual ~GeoLayer() {};
	virtual LayerType getLayerType()const { return Undefined; }
	virtual QString getName()const { return ""; }
	virtual int getLID()const { return -1; }
	virtual bool isVisible()const { return true; }
	virtual void setName(const QString&){}
	virtual void setLID(int){}
	virtual void setVisible(bool){}
	virtual void Draw()const{}


private:

};

class GeoFeatureLayer:public GeoLayer
{
public:
	GeoFeatureLayer();
	~GeoFeatureLayer();

private:

};

GeoFeatureLayer::GeoFeatureLayer()
{
}

GeoFeatureLayer::~GeoFeatureLayer()
{
}