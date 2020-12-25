#include "MainForm.h"

ZGIS::ZGIS(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(800, 600);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setWindowTitle(tr("ZGIS"));
    this->setWindowIcon(QIcon("resource/gis.png"));
    CreateWidget();
    CreateMenus();
    CreateStatusBar();
    CreateActions();
    CreateLayout();
}

ZGIS::~ZGIS()
{
}

void ZGIS::CreateWidget()
{
    layersTreeWidget = new LayerTreeWidget(this);
    toolboxTreeWidget = new ToolBoxTreeWidget(this);
    openGLWidget = new OpenGLWidget(this);
}

void ZGIS::CreateActions()
{
    //过于繁琐，应进行封装
    openShapeFileAction = new QAction(tr("Shape File"),this);
    openShapeFileAction->setIcon(QIcon("resource/shape.png"));
    openFileMenu->addAction(openShapeFileAction);
    connect(openShapeFileAction, &QAction::triggered, this, &ZGIS::OnOpenShapeFile);

    openGeoJsonAction = new QAction(tr("GeoJSON"), this);
    openGeoJsonAction->setIcon(QIcon("resource/json.png"));
    openFileMenu->addAction(openGeoJsonAction);

    openTiffAction = new QAction(tr("TIFF"),this);
    openTiffAction->setIcon(QIcon("resource/TIFF.png"));
    openFileMenu->addAction(openTiffAction);

    exitAction = new QAction(tr("Exit"),this);
    exitAction->setIcon(QIcon("resource/exit.png"));
    fileMenu->addAction(exitAction);

    drawLine = new QAction(tr("Draw Line"));
    drawLine->setIcon(QIcon("resource/line.png"));
    drawMenu->addAction(drawLine);

    clipLine = new QAction(tr("Clip Line"));
    clipLine->setIcon(QIcon("resource/line.png"));
    clipMenu->addAction(clipLine);

    aboutAction = new QAction(tr("About"));
    aboutAction->setIcon(QIcon("resource/about.png"));
    aboutMenu->addAction(aboutAction);
}

void ZGIS::CreateMenus()
{
    fileMenu = menuBar()->addMenu(tr("Flie"));
    openFileMenu = fileMenu->addMenu(tr("Open"));
    openFileMenu->setIcon(QIcon("resource/open.png"));
    editMenu = menuBar()->addMenu(tr("Edit"));
    drawMenu = editMenu->addMenu(tr("Input"));
    drawMenu->setIcon(QIcon("resource/input"));
    toolMenu = menuBar()->addMenu(tr("Tool"));
    clipMenu = toolMenu->addMenu(tr("Clip"));
    clipMenu->setIcon(QIcon("resource/clip"));
    aboutMenu = menuBar()->addMenu(tr("About"));

}

void ZGIS::CreateToolBar()
{
    
}

void ZGIS::CreateStatusBar()
{
   this->statusBar()->setStyleSheet("QStatusBar::item{border: 0px}");
   statusbar = new StatusBar(this);
   //目前好像statusbar没啥作用 12 24
}

void ZGIS::CreateLayout()
{
    QWidget* centerWidget = new QWidget();
    this->setCentralWidget(centerWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centerWidget);
    QVBoxLayout* leftLayout = new QVBoxLayout();

    mainLayout->setSpacing(6);
    leftLayout->addWidget(layersTreeWidget);
    leftLayout->addWidget(toolboxTreeWidget);
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(openGLWidget);
    mainLayout->setContentsMargins(11, 11, 11, 11);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 3);
}

void ZGIS::OnOpenShapeFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open File"), "", tr("Shape File(*.shp)"));
    if (files.isEmpty())
        return;
    for (auto iter = files.begin(); iter != files.end(); ++iter)
    {

    }
}
 