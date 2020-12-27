#include "GeoMap.h"

GeoMap::GeoMap(const GeoMap& rhs)
{
}

GeoMap::~GeoMap()
{
}

void GeoMap::Draw() const
{
    int layersCount = layers.size();
    for (int i = layersCount - 1; i > -1; --i) {
        auto* layer = getLayerByLID(layerOrders[i]);
        if (layer->GetLayerDefn()->IsGeometryIgnored())
            return;
    }
}

OGRLayer* GeoMap::getLayerByLID(int nLID) const
{
    return nullptr;
}
