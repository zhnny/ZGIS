#include "appevent.h"

AppEvent* AppEvent::pInstance = new AppEvent(nullptr);

AppEvent::AppEvent(QObject *parent) : QObject(parent)
{
}

AppEvent* AppEvent::getInstance() {
    return pInstance;
}

/*************************************************
 *
 *      Forward signals
 *
*************************************************/

void AppEvent::onNewMap(const QString& name, const QString& path) {
    emit sigNewMap(name, path);
}

void AppEvent::onUpdateOpengl() {
    emit sigUpdateOpengl();
}

void AppEvent::onAddNewLayerToLayersTree(OGRLayer* layer) {
    emit sigAddNewLayerToLayersTree(layer);
}

void AppEvent::onSendMapToGPU(bool bUpdate) {
    emit sigSendMapToGPU(bUpdate);
}

void AppEvent::onSendLayerToGPU(OGRLayer* layer, bool bUpdate/* = true*/) {
    emit sigSendLayerToGPU(layer, bUpdate);
}

void AppEvent::onSendFeatureToGPU(OGRFeature* feature) {
    emit sigSendFeatureToGPU(feature);
}

void AppEvent::onZoomToMap() {
    emit sigZoomToMap();
}

void AppEvent::onZoomToLayer(OGRLayer* layer) {
    emit sigZoomToLayer(layer);
}

void AppEvent::onUpdateCoord(double geoX, double geoY) {
    emit sigUpdateCoord(geoX, geoY);
}

void AppEvent::onStartEditing(bool on) {
    emit sigStartEditing(on);
}

void AppEvent::onUpdateCursorType() {
    emit sigUpdateCursorType();
}

void AppEvent::onDeleteFeatures(bool softDelete) {
    emit sigDeleteFeatures(softDelete);
}

