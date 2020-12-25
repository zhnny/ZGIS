#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qfiledialog.h>
#include "StatusBar.h"
#include "ToolBoxTreeWidget.h"
#include "OpenGLWidget.h"
#include "LayerTreeWidget.h"

class ZGIS : public QMainWindow
{
    Q_OBJECT

public:
    ZGIS(QWidget *parent = Q_NULLPTR);
    ~ZGIS();
    void CreateWidget();
    void CreateActions();
    void CreateMenus();
    void CreateToolBar();
    void CreateStatusBar();
    void CreateLayout();

public slots:
    void OnOpenShapeFile();

private:
    QMenu* fileMenu;
    QMenu* openFileMenu;
    QMenu* editMenu;
    QMenu* drawMenu;
    QMenu* toolMenu;
    QMenu* clipMenu;
    QMenu* aboutMenu;

    QAction* openShapeFileAction;
    QAction* openTiffAction;
    QAction* openGeoJsonAction;
    QAction* exitAction;
    QAction* drawLine;
    QAction* clipLine;
    QAction* aboutAction;

    QStatusBar* statusbar;
    LayerTreeWidget* layersTreeWidget;
    ToolBoxTreeWidget* toolboxTreeWidget;
    OpenGLWidget* openGLWidget;
    
};
