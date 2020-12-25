#pragma once

#include <QtCore/qstring.h>
#include "GeoFieldDefine.h"

class GeoFeatureLayerProperty
{
public:
	GeoFeatureLayerProperty();
	~GeoFeatureLayerProperty();
	void setName(const QString& nameInput) { name = nameInput; }

public:
	bool visible = true;
	int id = 0;
	QString name;
	QString apstialRef;

private:

};

GeoFeatureLayerProperty::GeoFeatureLayerProperty()
{
}

GeoFeatureLayerProperty::~GeoFeatureLayerProperty()
{
}