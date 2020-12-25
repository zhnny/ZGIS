#pragma once
#include <QtGui/qcolor.h>
#include <vector>
#include "GeoFieldDefine.h"

class GeoFeatureLayer;
class OpenGLFeatureDescriptor;

class GeoFeature
{
public:
	GeoFeature() {};
	~GeoFeature() {};
	GeoFeature(GeoFeatureLayer* layerParent);
	GeoFeature(int FID, GeoFeatureLayer* layerParent);
	GeoFeature(std::vector<GeoFieldDefine*>* fieldDefinesInput);

private:

};
