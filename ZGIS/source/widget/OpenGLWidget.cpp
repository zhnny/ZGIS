#include "OpenGLWidget.h"
#include <opengl/glcall.h>
OGRLayer* tempLayer = nullptr;
OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent), map(Env::map)
{
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::ClickFocus);

	connect(this, &OpenGLWidget::signalUpadteCoord,
		AppEvent::getInstance(), &AppEvent::onUpdateCoord);
	connect(AppEvent::getInstance(), &AppEvent::sigZoomToMap,
		this, &OpenGLWidget::onZoomToMap);
	connect(AppEvent::getInstance(), &AppEvent::sigZoomToLayer,
		this, &OpenGLWidget::onZoomToLayer);
	connect(AppEvent::getInstance(), &AppEvent::sigUpdateOpengl,
		this, [this] { update(); });
	connect(AppEvent::getInstance(), &AppEvent::sigSendMapToGPU,
		this, &OpenGLWidget::onSendMapToGPU);
	connect(AppEvent::getInstance(), &AppEvent::sigSendLayerToGPU,
		this, &OpenGLWidget::onSendLayerToGPU);
	connect(AppEvent::getInstance(), &AppEvent::sigSendFeatureToGPU,
		this, &OpenGLWidget::onSendFeatureToGPU);
}

OpenGLWidget::~OpenGLWidget()
{
	makeCurrent();
	isRunning = false;
}

void OpenGLWidget::onSendMapToGPU(bool update)
{
	for (auto iter = map->begin(); iter != map->end(); ++iter)
		onSendLayerToGPU(*iter, false);
	if (update)
		this->update();
}

void OpenGLWidget::onSendDataSourceToGPU(OGRDataSource* dataSource, bool update)
{
	if (dataSource != NULL)
	{
		int layerCount = dataSource->GetLayerCount();
		for (int i = layerCount - 1; i > -1; i--)
		{
			onSendLayerToGPU(dataSource->GetLayer(i), update);
		}
		map->addLayer(tempLayer);
		map->updateExtent();
	}
}

void OpenGLWidget::onSendLayerToGPU(OGRLayer* layer, bool update)
{
	//目前只处理矢量图层
	tempLayer = layer;
	onSendFeatureLayerToGPU(layer, update);

}

void OpenGLWidget::onSendFeatureLayerToGPU(OGRLayer* layer, bool update)
{
	if (!layer)
		return;
	makeCurrent();
	int featureCount = layer->GetFeatureCount();
	for (int i = featureCount - 1; i > -1; i--)
	{
		OGRFeature* feature = layer->GetFeature(i);
		onSendFeatureToGPU(feature);
	}

	if (map->getNumLayers() == 1)
		updateMVP(true, false, true);
	else
		updateMVP(false, false, true);

	setMVP();

	if (update) {
		this->update();
	}
}

void OpenGLWidget::onSendFeatureToGPU(OGRFeature* feature)
{
	OGRGeometry* geometry = feature->GetGeometryRef();
	OGRwkbGeometryType wkbType = geometry->getGeometryType();
	float r = 0.0, g = 0.0, b = 0.0;
	//feature->getColorF(r, g, b);

	makeCurrent();
	OpenglFeatureDescriptor* featureDesc = nullptr;

	switch (wkbType) {
	default:
		break;
	case wkbPoint:
	{
		OGRPoint* point = geometry->toPoint();
		featureDesc = sendPointToGPU(point, r, g, b);
		break;
	}
	case wkbPolygon:
	{
		OGRPolygon* polygon = geometry->toPolygon();
		featureDesc = sendPolygonToGPU(polygon, r, g, b);
		break;
	}
	case wkbLineString:
	{
		OGRLineString* lineString = geometry->toLineString();
		featureDesc = sendLineStringToGPU(lineString, r, g, b);
		break;
	}
	case wkbMultiPoint:
	{
		OGRMultiPoint* multiPoint = geometry->toMultiPoint();
		featureDesc = sendMultiPointToGPU(multiPoint, r, g, b);
		break;
	}
	case wkbMultiPolygon:
	{
		OGRMultiPolygon* multiPolygon = geometry->toMultiPolygon();
		featureDesc = sendMultiPolygonToGPU(multiPolygon, r, g, b);
		break;
	}
	case wkbMultiLineString:
	{
		OGRMultiLineString* multiLineString = geometry->toMultiLineString();
		featureDesc = sendMultiLineStringToGPU(multiLineString, r, g, b);
		break;
	}
	}

	map->setOpenglFeatureDescriptor(featureDesc);
}

void OpenGLWidget::onZoomToLayer(OGRLayer* layer)
{
}

void OpenGLWidget::onZoomToMap()
{
}

OpenglFeatureDescriptor* OpenGLWidget::sendPointToGPU(OGRPoint* point, float r, float g, float b)
{
	OpenglFeatureDescriptor* featureDesc = new OpenglFeatureDescriptor(5);
	auto& vao = featureDesc->vao;
	auto& vbo = featureDesc->vbo;
	auto& ibos = featureDesc->ibos;

	// VAO
	vao = new VertexArray();

	// VBO
	float vertices[5] = { float(point->getX()), float(point->getY()), r, g, b };
	vbo = new VertexBuffer(vertices, 5 * sizeof(float));

	// IBO
	unsigned int indices = 0;
	IndexBuffer* ibo = new IndexBuffer(&indices, 1, GL_POINTS);
	ibos.emplace_back(ibo);

	// Data layout
	VertexBufferLayout layout;
	layout.Push<float>(2);	// x, y
	layout.Push<float>(3);	// r, g, b
	vao->addBuffer(*vbo, layout);

	return featureDesc;
}

OpenglFeatureDescriptor* OpenGLWidget::sendMultiPointToGPU(OGRMultiPoint* mutliPoint, float r, float g, float b)
{
	OpenglFeatureDescriptor* featureDesc = new OpenglFeatureDescriptor(5);
	int pointsCount = mutliPoint->getNumGeometries();
	auto& vao = featureDesc->vao;
	auto& vbo = featureDesc->vbo;
	auto& ibos = featureDesc->ibos;

	// VAO
	vao = new VertexArray();

	// VBO
	float* vertices = new float[pointsCount * 5 * sizeof(float)];
	OGRPoint* point;
	for (int i = 0; i < pointsCount; ++i) {
		point = mutliPoint->getGeometryRef(i)->toPoint();//未测试12 28
		vertices[i * 5] = point->getX();
		vertices[i * 5 + 1] = point->getY();
		vertices[i * 5 + 2] = r;
		vertices[i * 5 + 3] = g;
		vertices[i * 5 + 4] = b;
	}
	vbo = new VertexBuffer(vertices, pointsCount * 5 * sizeof(float));

	// IBO
	unsigned int* indices = utils::newContinuousNumber(0, pointsCount);
	IndexBuffer* ibo = new IndexBuffer(indices, pointsCount, GL_POINTS);
	ibos.emplace_back(ibo);
	delete[] vertices;
	delete[] indices;

	// Data layout
	VertexBufferLayout layout;
	layout.Push<float>(2);	// x, y
	layout.Push<float>(3);	// r, g, b
	vao->addBuffer(*vbo, layout);

	return featureDesc;
}

OpenglFeatureDescriptor* OpenGLWidget::sendLineStringToGPU(OGRLineString* lineString, float r, float g, float b)
{
	OpenglFeatureDescriptor* featureDesc = new OpenglFeatureDescriptor(5);
	int pointsCount = lineString->getNumPoints();
	auto& vao = featureDesc->vao;
	auto& vbo = featureDesc->vbo;
	auto& ibos = featureDesc->ibos;

	// VAO
	vao = new VertexArray();

	// VBO
	float* vertices = new float[pointsCount * 5];
	for (int i = 0; i < pointsCount; ++i) {
		vertices[i * 5] = lineString->getX(i);
		vertices[i * 5 + 1] = lineString->getY(i);
		vertices[i * 5 + 2] = r;
		vertices[i * 5 + 3] = g;
		vertices[i * 5 + 4] = b;
	}
	vbo = new VertexBuffer(vertices, 5 * sizeof(float) * pointsCount);

	// IBO
	unsigned int* indices = utils::newContinuousNumber(0, pointsCount);
	IndexBuffer* ibo = new IndexBuffer(indices, pointsCount, GL_LINE_STRIP);
	ibos.emplace_back(ibo);
	delete[] vertices;
	delete[] indices;

	// Data layout
	VertexBufferLayout layout;
	layout.Push<float>(2);	// x, y
	layout.Push<float>(3);	// r, g, b
	vao->addBuffer(*vbo, layout);

	return featureDesc;
}

OpenglFeatureDescriptor* OpenGLWidget::sendMultiLineStringToGPU(OGRMultiLineString* multiLineString, float r, float g, float b)
{
	OpenglFeatureDescriptor* featureDesc = new OpenglFeatureDescriptor(5);
	int pointsCount = multiLineString->toMultiPoint()->getNumGeometries();
	int linesCount = multiLineString->getNumGeometries();
	auto& vao = featureDesc->vao;
	auto& vbo = featureDesc->vbo;
	auto& ibos = featureDesc->ibos;

	// VAO
	vao = new VertexArray();

	// VBO
	vbo = new VertexBuffer(nullptr, 5 * sizeof(float) * pointsCount);

	int sizeOffset = 0;
	int countOffset = 0;

	for (int i = 0; i < linesCount; ++i) {
		OGRLineString* lineString = multiLineString->getGeometryRef(i)->toLineString();

		float* vertices = new float[pointsCount * 5];
		for (int i = 0; i < pointsCount; ++i) {
			vertices[i * 5] = lineString->getX(i);
			vertices[i * 5 + 1] = lineString->getY(i);
			vertices[i * 5 + 2] = r;
			vertices[i * 5 + 3] = g;
			vertices[i * 5 + 4] = b;
		}
		vbo->addSubData(vertices, sizeOffset, pointsCount * 5 * sizeof(float));

		// IBO
		unsigned int* indices = utils::newContinuousNumber(countOffset, pointsCount);
		IndexBuffer* ibo = new IndexBuffer(indices, pointsCount, GL_LINE_STRIP);
		ibos.push_back(ibo);
		sizeOffset += pointsCount * 5 * sizeof(float);
		countOffset += pointsCount;
		delete[] vertices;
		delete[] indices;
	}

	// Data layout
	VertexBufferLayout layout;
	layout.Push<float>(2);	// x, y
	layout.Push<float>(3);	// r, g, b
	vao->addBuffer(*vbo, layout);

	return featureDesc;
}

OpenglFeatureDescriptor* OpenGLWidget::sendPolygonToGPU(OGRPolygon* polygon, float r, float g, float b)
{
	OpenglFeatureDescriptor* featureDesc = new OpenglFeatureDescriptor(8);
	int polygonPointsCount = polygon->toMultiPoint()->getNumGeometries();
	int interiorRingsCount = polygon->getNumInteriorRings();
	auto& vao = featureDesc->vao;
	auto& vbo = featureDesc->vbo;
	auto& ibos = featureDesc->ibos;

	// VAO
	vao = new VertexArray();
	vao->reserve(interiorRingsCount + 1);

	float* vertices = new float[polygonPointsCount * 8 * sizeof(float)];
	int iStride = 0;

	using Point = std::array<double, 2>;
	std::vector<std::vector<Point>> vPolygon;

	// Exterior ring
	OGRLinearRing* geoExteriorRing = polygon->getExteriorRing();
	int exteriorRingPointsCount = geoExteriorRing->getNumPoints();
	vao->setStride(iStride++, exteriorRingPointsCount);
	std::vector<Point> exteriorRing;
	exteriorRing.reserve(exteriorRingPointsCount);
	GeoRawPoint rawPoint;
	OGRPoint* pointTemp=new OGRPoint();
	int index = 0;
	for (int i = 0; i < exteriorRingPointsCount; ++i) {
		//geoExteriorRing->getRawPoint(i, &rawPoint);
		geoExteriorRing->getPoint(i, pointTemp);
		rawPoint.x = pointTemp->getX();
		rawPoint.y = pointTemp->getY();
		exteriorRing.push_back({ rawPoint.x,rawPoint.y });
		// position
		vertices[index] = rawPoint.x;
		vertices[index + 1] = rawPoint.y;
		// fill color
		vertices[index + 2] = r;
		vertices[index + 3] = g;
		vertices[index + 4] = b;
		// border color
		// default: black border
		vertices[index + 5] = 0.0f;
		vertices[index + 6] = 0.0f;
		vertices[index + 7] = 0.0f;
		index += 8;
	}
	vPolygon.emplace_back(exteriorRing);

	// Interior rings
	for (int j = 0; j < interiorRingsCount; ++j) {
		const auto& geoInteriorRing = polygon->getInteriorRing(j);
		int interiorRingPointsCount = geoInteriorRing->getNumPoints();
		vao->setStride(iStride++, interiorRingPointsCount);
		std::vector<Point> interiorRing;
		interiorRing.reserve(interiorRingPointsCount);
		for (int k = 0; k < interiorRingPointsCount; ++k) {
			geoInteriorRing->getPoint(k, pointTemp);
			rawPoint.x = pointTemp->getX();
			rawPoint.y = pointTemp->getY();
			exteriorRing.push_back({ rawPoint.x, rawPoint.y });
			// position
			vertices[index] = rawPoint.x;
			vertices[index + 1] = rawPoint.y;
			// fill color
			vertices[index + 2] = r;
			vertices[index + 3] = g;
			vertices[index + 4] = b;
			// border color
			// default: black border
			vertices[index + 5] = 0.0f;
			vertices[index + 6] = 0.0f;
			vertices[index + 7] = 0.0f;
			index += 8;
		}
		vPolygon.emplace_back(interiorRing);
	}
	// Triangulation
	std::vector<unsigned int> indices = mapbox::earcut<unsigned int>(vPolygon);
	//std::reverse(indices.begin(), indices.end());

	// VBO
	vbo = new VertexBuffer(vertices, polygonPointsCount * 8 * sizeof(float));

	// IBO
	IndexBuffer* ibo = new IndexBuffer(&indices[0], indices.size(), GL_TRIANGLES);
	ibos.push_back(ibo);
	delete[] vertices;

	// Data layout
	VertexBufferLayout layout;
	layout.Push<float>(2);	// x, y
	layout.Push<float>(3);	// Fill   color: r, g, b
	layout.Push<float>(3);	// Border color: r, g, b
	vao->addBuffer(*vbo, layout);

	return featureDesc;
}

OpenglFeatureDescriptor* OpenGLWidget::sendMultiPolygonToGPU(OGRMultiPolygon* multiPolygon, float r, float g, float b)
{
	//
	//不确定获得多边形点数的函数是否有问题12 29 以调试来看，是有问题的
	//
	OpenglFeatureDescriptor* featureDesc = new OpenglFeatureDescriptor(8);
	int pointsCount = multiPolygon->toMultiPoint()->getNumGeometries();
	int polygonCount = multiPolygon->getNumGeometries();
	int linearRingsCount = multiPolygon->toMultiLineString()->getNumGeometries();
	auto& vao = featureDesc->vao;
	auto& vbo = featureDesc->vbo;
	auto& ibos = featureDesc->ibos;

	// VAO
	vao = new VertexArray();
	vao->reserve(linearRingsCount);

	int sizeOffset = 0;
	int countOffset = 0;

	// VBO
	// Just allocate memory, no data was send.
	vbo = new VertexBuffer(nullptr, pointsCount * 8 * sizeof(float));
	int iStride = 0;

	for (int i = 0; i < polygonCount; ++i) {
		OGRPolygon* geoPolygon = multiPolygon->getGeometryRef(i)->toPolygon();
		int polygonPointsCount = geoPolygon->toLineString()->getNumPoints();
		int interiorRingsCount = geoPolygon->getNumInteriorRings();

		float* vertices = new float[polygonPointsCount * 5 * sizeof(float)];

		using Point = std::array<double, 2>;
		std::vector<std::vector<Point>> polygon;

		// Exterior ring
		OGRLinearRing* geoExteriorRing = geoPolygon->getExteriorRing();
		int exteriorRingPointsCount = geoExteriorRing->getNumPoints();
		vao->setStride(iStride++, exteriorRingPointsCount);
		std::vector<Point> exteriorRing;
		exteriorRing.reserve(exteriorRingPointsCount);
		GeoRawPoint rawPoint;
		int index = 0;
		OGRPoint* pointTemp = new OGRPoint();

		for (int i = 0; i < exteriorRingPointsCount; ++i) {
			geoExteriorRing->getPoint(i, pointTemp);
			rawPoint.x = pointTemp->getX();
			rawPoint.y = pointTemp->getY();
			exteriorRing.push_back({ rawPoint.x, rawPoint.y });
			// position
			vertices[index] = rawPoint.x;
			vertices[index + 1] = rawPoint.y;
			// fill color
			vertices[index + 2] = r;
			vertices[index + 3] = g;
			vertices[index + 4] = b;
			// border color
			// default: black border
			vertices[index + 5] = 0.0f;
			vertices[index + 6] = 0.0f;
			vertices[index + 7] = 0.0f;
			index += 8;
		}
		polygon.emplace_back(exteriorRing);

		// Interior rings
		for (int j = 0; j < interiorRingsCount; ++j) {
			const auto& geoInteriorRing = geoPolygon->getInteriorRing(j);
			int interiorRingPointsCount = geoInteriorRing->getNumPoints();
			vao->setStride(iStride++, interiorRingPointsCount);
			std::vector<Point> interiorRing;
			interiorRing.reserve(interiorRingPointsCount);
			for (int k = 0; k < interiorRingPointsCount; ++k) {
				geoInteriorRing->getPoint(i, pointTemp);
				rawPoint.x = pointTemp->getX();
				rawPoint.y = pointTemp->getY();
				exteriorRing.push_back({ rawPoint.x, rawPoint.y });
				// position
				vertices[index] = rawPoint.x;
				vertices[index + 1] = rawPoint.y;
				// fill color
				vertices[index + 2] = r;
				vertices[index + 3] = g;
				vertices[index + 4] = b;
				// border color
				// default: black border
				vertices[index + 5] = 0.0f;
				vertices[index + 6] = 0.0f;
				vertices[index + 7] = 0.0f;
				index += 8;
			}
			polygon.emplace_back(interiorRing);
		}

		// Triangulation
		std::vector<unsigned int> indices = mapbox::earcut<unsigned int>(polygon);
		//std::reverse(indices.begin(), indices.end());

		if (countOffset > 0)
			std::for_each(indices.begin(), indices.end(), [&countOffset](auto& value) {value += countOffset; });

		// VBO
		vbo->addSubData(vertices, sizeOffset, polygonPointsCount * 8 * sizeof(float));

		// IBO
		IndexBuffer* ibo = new IndexBuffer(&indices[0], indices.size(), GL_TRIANGLES);
		ibos.push_back(ibo);
		sizeOffset += polygonPointsCount * 8 * sizeof(float);
		countOffset += polygonPointsCount;
		delete[] vertices;
	}
	//Data layout
		VertexBufferLayout layout;
	layout.Push<float>(2);	// x, y
	layout.Push<float>(3);	// Fill   color: r, g, b
	layout.Push<float>(3);	// Border color: r, g, b
	vao->addBuffer(*vbo, layout);

	return featureDesc;
}

void OpenGLWidget::initializeGL()
{
	if (glewInit() != GLEW_NO_ERROR)
	{
		return;
	}
	Env::createShaders();
	// view matrix
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void OpenGLWidget::resizeGL(int w, int h)
{
	GLCall(glViewport(0, 0, w, h));
	GLCall(glMatrixMode(GL_PROJECTION));

	if (!map || map->isEmpty())
		return;
	if (h < 1)
		h = 1;

	// projection matrix
	float aspectRatio = float(w) / h;
	GeoExtent mapExtent = map->getExtent();
	if (mapExtent.aspectRatio() < aspectRatio) {
		adjustedMapExtent.minX = (mapExtent.minX - mapExtent.centerX()) * aspectRatio / mapExtent.aspectRatio() + mapExtent.centerX();
		adjustedMapExtent.maxX = (mapExtent.maxX - mapExtent.centerX()) * aspectRatio / mapExtent.aspectRatio() + mapExtent.centerX();
	}
	else {
		adjustedMapExtent.minY = (mapExtent.minY - mapExtent.centerY()) * mapExtent.aspectRatio() / aspectRatio + mapExtent.centerY();
		adjustedMapExtent.maxY = (mapExtent.maxY - mapExtent.centerY()) * mapExtent.aspectRatio() / aspectRatio + mapExtent.centerY();
	}
	proj = glm::ortho(float(adjustedMapExtent.minX), float(adjustedMapExtent.maxX),
		float(adjustedMapExtent.minY), float(adjustedMapExtent.maxY),
		-1.0f, 1.0f);

	// MVP
	setMVP();

	update();
}

void OpenGLWidget::paintGL()
{
	Env::renderer.Clear();

	if (!isRunning || !map || map->isEmpty())
		return;

	map->Draw();//未测试12 28

	if (isRectSelecting) {
		GLCall(glUseProgram(0));
		drawRectNoFill(mouseBeginPos, mouseCurrPos, 1.0f, 0.5f, 0.0f, 5);
		drawRectFillColor(mouseBeginPos, mouseCurrPos, 1.0f, 0.5f, 0.0f, 0.5f);
	}
}

void OpenGLWidget::mousePressEvent(QMouseEvent* ev)
{
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* ev)
{
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* ev)
{
}

void OpenGLWidget::wheelEvent(QWheelEvent* ev)
{
}

void OpenGLWidget::enterEvent(QEvent*)
{
}

void OpenGLWidget::keyPressEvent(QKeyEvent* ev)
{
}

void OpenGLWidget::updateMVP(bool updateModel, bool updateView, bool updateProj)
{
	GeoExtent mapExtent = map->getExtent();
	adjustedMapExtent = mapExtent;

	// model matrix
	if (updateModel) {
		// model
		// translation
		xOffset = 0.0f;
		yOffset = 0.0f;
		trans = glm::translate(glm::mat4(1.0f), glm::vec3(xOffset, yOffset, 0.0f));
		// sacle
		zoom = 1.0f;
		scale = glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, 1.0f));
		model = trans * scale;
	}

	// view matrix
	if (updateView) {
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	// projection matrix
	if (updateProj) {
		float aspectRatio = float(this->width()) / this->height();
		if (mapExtent.aspectRatio() < aspectRatio) {
			adjustedMapExtent.minX = (mapExtent.minX - mapExtent.centerX()) * aspectRatio / mapExtent.aspectRatio() + mapExtent.centerX();
			adjustedMapExtent.maxX = (mapExtent.maxX - mapExtent.centerX()) * aspectRatio / mapExtent.aspectRatio() + mapExtent.centerX();
		}
		else {
			adjustedMapExtent.minY = (mapExtent.minY - mapExtent.centerY()) * mapExtent.aspectRatio() / aspectRatio + mapExtent.centerY();
			adjustedMapExtent.maxY = (mapExtent.maxY - mapExtent.centerY()) * mapExtent.aspectRatio() / aspectRatio + mapExtent.centerY();
		}
		proj = glm::ortho(float(adjustedMapExtent.minX), float(adjustedMapExtent.maxX),
			float(adjustedMapExtent.minY), float(adjustedMapExtent.maxY),
			-1.0f, 1.0f);
	}
}

void OpenGLWidget::setMVP()
{
	glm::mat4 mvp = proj * view * model;

	Env::pointShader.Bind();
	Env::pointShader.SetUniformMat4f("u_MVP", mvp);

	Env::lineShader.Bind();
	Env::lineShader.SetUniformMat4f("u_MVP", mvp);

	Env::polygonShader.Bind();
	Env::polygonShader.SetUniformMat4f("u_MVP", mvp);

	Env::borderShader.Bind();
	Env::borderShader.SetUniformMat4f("u_MVP", mvp);

	Env::highlightShader.Bind();
	Env::highlightShader.SetUniformMat4f("u_MVP", mvp);

	Env::textureShader.Bind();
	Env::textureShader.SetUniformMat4f("u_MVP", mvp);
}

void OpenGLWidget::drawRectNoFill(const QPoint& startPoint, const QPoint& endPonit, float r, float g, float b, float a)
{
}

void OpenGLWidget::drawRectFillColor(const QPoint& startPoint, const QPoint& endPoint, float r, float g, float b, int lineWidth)
{
}
