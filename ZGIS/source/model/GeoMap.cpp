#include "GeoMap.h"

GeoMap::GeoMap(const GeoMap& rhs)
    : properties(rhs.properties), layerOrders(rhs.layerOrders)
{
    int layersCount = rhs.layers.size();
    layers.reserve(layersCount);
    for (int i = 0; i < layersCount; ++i) {
        layers.push_back(rhs.layers[i]);
    }

}
GeoMap::~GeoMap()
{
    for (auto& layer : layers)
        delete layer;
}

GeoMap* GeoMap::copy()
{
    return new GeoMap(*this);
}

void GeoMap::Draw() const
{
    int layersCount = layers.size();
    for (int i = layersCount - 1; i > -1; --i) {
        auto* layer = getLayerByLID(layerOrders[i]);
        if (true)
            Draw(layer);
    }
}

void GeoMap::Draw(OGRLayer* layer) const
{
    int featureCount = layer->GetFeatureCount();
    for (int i = featureCount - 1; i > -1;--i) {
        OGRFeature* feature = layer->GetFeature(i);
        Draw(feature);
    }
}

void GeoMap::Draw(OGRFeature* feature) const
{
    OGRGeometry* geometry = feature->GetGeometryRef();
    Draw(geometry);
}

void GeoMap::Draw(OGRGeometry* geometry) const
{
    OGRwkbGeometryType type=geometry->getGeometryType();
    switch (type)
    {
    default:
        break;
    case wkbPoint:
    case wkbMultiPoint: {
        for (const auto& ibo : openglFeatureDesc->ibos) {
            Env::renderer.DrawPoint(openglFeatureDesc->vao, ibo, Env::pointShader);
        }
        break;
    }
    case wkbLineString:
    case wkbMultiLineString: {
        for (const auto& ibo : openglFeatureDesc->ibos) {
            Env::renderer.DrawLine(openglFeatureDesc->vao, ibo, Env::lineShader);
        }
        break;
    }
    case wkbPolygon:
    case wkbMultiPolygon: {
        /* Fill Color */
        for (const auto& ibo : openglFeatureDesc->ibos) {
            Env::renderer.DrawPolygon(openglFeatureDesc->vao, ibo, Env::polygonShader);
        }
        /* Draw border */
        Env::renderer.DrawPolygonBorder(openglFeatureDesc->vao, Env::borderShader);
        break;
    }
    }
}

OGRLayer* GeoMap::getLayerByOrder(int order) const
{
    int nLID = layerOrders[order];
    return getLayerByLID(nLID);
}

OGRLayer* GeoMap::getLayerByLID(int nLID) const
{
    int idx = getLayerIndexByLID(nLID);
    if (idx != -1)
        return layers[idx];
    else
        return nullptr;
}

OGRLayer* GeoMap::getLayerByName(const QString& name) const
{
    int idx = getLayerIndexByName(name);
    if (idx != -1)
        return layers[idx];
    else
        return nullptr;
}

// The last layer
OGRLayer* GeoMap::getLastLayer() const
{
    if (layers.empty())
        return nullptr;
    else
        return layers.back();
}

int GeoMap::getLayerIndexByOrder(int order) const
{
    int nLID = layerOrders[order];
    return getLayerIndexByLID(nLID);
}
/// <summary>
/// 没有作用
/// </summary>
/// <param name="nLID">LID数字</param>
/// <returns>index</returns>
int GeoMap::getLayerIndexByLID(int nLID) const
{
    int count = layers.size();
    return nLID;
}

int GeoMap::addLayer(OGRLayer* layerInput)
{
    // Put the newly added layer on top
    layerOrders.push_front(currentLID);
    //layerInput->setLID(currentLID++);
    
    OGREnvelope* envlope = new OGREnvelope();
    layerInput->GetExtent(envlope);
    // Get and cache map's extent
    if (isEmpty())
    {
        properties.extent.minX = envlope->MinX;
        properties.extent.minY = envlope->MinY;
        properties.extent.maxX = envlope->MaxX;
        properties.extent.maxY = envlope->MaxY;
    }
    else
    {
        GeoExtent* extent = new GeoExtent(envlope->MinX, envlope->MaxX, envlope->MinY, envlope->MaxY);
        properties.extent.merge(*extent);
    }
    layers.push_back(layerInput);

    // return the newly added layer's ID (LID)
    return currentLID - 1;

}
/// <summary>
/// 未实现
/// </summary>
/// <param name="nLID"></param>
/// <returns></returns>
bool GeoMap::removeLayerByLID(int nLID)
{
    return false;
}
/// <summary>
/// 未实现
/// </summary>
/// <param name="nLID"></param>
/// <param name="insertLID"></param>
void GeoMap::changeLayerOrder(int nLID, int insertLID)
{
}

int GeoMap::getLayerIndexByName(const QString& name) const
{
    int count = layers.size();
    for (int i = 0; i < count; ++i) {
        if (layers[i]->GetName() == name)
            return i;
    }
    return -1;
}

void GeoMap::updateExtent()
{
    if (isEmpty()) {
        properties.extent = GeoExtent();
    }
    else {
        OGREnvelope* envlope = new OGREnvelope();
        layers[0]->GetExtent(envlope);
        GeoExtent* extent = new GeoExtent(envlope->MinX, envlope->MaxX, envlope->MinY, envlope->MaxY);
        properties.extent = *extent;
        int count = layers.size();
        for (int i = 1; i < count; ++i) {
            layers[i]->GetExtent(envlope);
            extent->minX = envlope->MinX;
            extent->minY = envlope->MinY;
            extent->maxX = envlope->MaxX;
            extent->maxX = envlope->MaxX;
            properties.extent.merge(*extent);
            // or
            //properties.extent += layers[i]->getExtent();
        }
    }
}

void GeoMap::setOpenglFeatureDescriptor(OpenglFeatureDescriptor* desc)
{
    if (openglFeatureDesc)
        delete openglFeatureDesc;
    openglFeatureDesc = desc;
}
