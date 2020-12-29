#pragma once
#include <gdal/ogrsf_frmts.h>
#include <vector>
#include <deque>
#include <QtCore/qstring.h>
#include "GeoMapProperty.h"
#include "opengl/openglfeaturedescriptor.h"
#include "opengl/env.h"

class GeoMap
{
public:
	struct LayerOrder
	{
		LayerOrder(int nLID, int nOrder) :LID(nLID), order(nOrder){}
		int LID;
		int order;
	};

public:
	GeoMap() {};
	GeoMap(const GeoMap& rhs);
	~GeoMap();
	GeoMap* copy();
	void Draw() const;
	void Draw(OGRLayer* layer)const;
	void Draw(OGRFeature* feature)const;
	void Draw(OGRGeometry* geometry)const;
	/// <summary>
	/// OGRLayer
	/// </summary>
	bool isEmpty()const { return layers.empty(); }
	int getNumLayers()const { return layers.size(); }
	OGRLayer* getLayerByOrder(int order)const;
	OGRLayer* getLayerByID(int id)const { return layers[id]; }
	OGRLayer* getLayerByLID(int nLID)const;
	OGRLayer* getLayerByName(const QString& name)const;
	OGRLayer* getLastLayer()const;
	int getLayerLIDByOrder(int order)const { return layerOrders[order]; }
	int getLayerIndexByOrder(int order)const;
	int getLayerIndexByName(const QString& name)const;
	int getLayerIndexByLID(int lid)const;
	
	int addLayer(OGRLayer* layerInput);
	bool removeLayerByLID(int nLID);
	void changeLayerOrder(int nLID, int insertLID);

	std::vector<OGRLayer*>::iterator begin() { return layers.begin(); }
	std::vector<OGRLayer*>::iterator end() { return layers.end(); }

	/// <summary>
	/// property
	/// </summary>
	QString getName()const { return properties.name; }
	GeoExtent getExtent()const { return properties.extent; }
	void setName(const QString& nameInput) { properties.name = nameInput; }
	void updateExtent();
	void setOpenglFeatureDescriptor(OpenglFeatureDescriptor* openglFeatureDesc);

	void queryFeature(double x, double y, double halfEdge, OGRLayer*& layerOut, OGRFeature*& featureOut);
	void queryFeatures(const GeoExtent& extent, std::map<OGRLayer*, std::vector<OGRFeature*>>& featuresOut);

	void getAllSelectedFeatures(std::map <OGRLayer*, std::vector<OGRFeature*>>& selectedFeatures);
	void emplaceSelectedFeature(int nLID, OGRFeature* selectedFeature);
	void emplaceSelectedFeatures(int nLID, const std::vector<OGRFeature*>& selectedFeatures);
	void emplaceSelectedFeatures(OGRLayer* layer, const std::vector<OGRFeature*>& selectedFeatures);
	void setSelectedFeatures(const std::map<OGRLayer*, std::vector<OGRFeature*>>& selectedFeatures);
	void clearSelectedFeatures();
	void offsetSelectedFeatures(double xOffset, double yOffset);

private:
	int currentLID = 0;
	std::vector<OGRLayer*> layers;
	std::deque<int> layerOrders;
	GeoMapProperty properties;
	OpenglFeatureDescriptor* openglFeatureDesc = nullptr;
};
