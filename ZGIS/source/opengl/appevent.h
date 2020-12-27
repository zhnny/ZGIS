#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>

class OGRLayer;
class OGRFeature;


class AppEvent : public QObject
{
    Q_OBJECT
public:
    static AppEvent* getInstance();

signals:
    void sigNewMap(const QString& name, const QString& path);
    void sigUpdateOpengl();
    void sigAddNewLayerToLayersTree(OGRLayer* layer);
    void sigSendMapToGPU(bool bUpdate = true);
    void sigSendLayerToGPU(OGRLayer* layer, bool bUpdate = true);
    void sigSendFeatureToGPU(OGRFeature* feature);
    void sigZoomToMap();
    void sigZoomToLayer(OGRLayer* layer);
    void sigUpdateCoord(double geoX, double geoY);
    void sigStartEditing(bool on);
    void sigUpdateCursorType();
    void sigDeleteFeatures(bool softDelete);

public slots:
    void onNewMap(const QString& name, const QString& path);
    void onUpdateOpengl();
    void onAddNewLayerToLayersTree(OGRLayer* layer);
    void onSendMapToGPU(bool bUpdate = true);
    void onSendLayerToGPU(OGRLayer* layer, bool bUpdate = true);
    void onSendFeatureToGPU(OGRFeature* feature);
    void onZoomToMap();
    void onZoomToLayer(OGRLayer* layer);
    void onUpdateCoord(double geoX, double geoY);
    void onStartEditing(bool on);
    void onUpdateCursorType();
    void onDeleteFeatures(bool softDelete);

private:
    explicit AppEvent(QObject *parent = nullptr);
    static AppEvent* pInstance;
};

#endif // APPEVENT_H
