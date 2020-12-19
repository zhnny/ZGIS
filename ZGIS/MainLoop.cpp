#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shapefil.h"
#include "LineClipping.h"

using namespace std;

const char *shpPath = "data\\bou2_4l.shp";

const int nVBOs = 3;
unsigned int VBO[nVBOs];
unsigned int VAO[nVBOs];

Shader *pShader = NULL;

int winWidth = 600, winHeight = 600;
bool bLButtonDown = false;
bool firstMouse = true;
double xx0, yy0, xx1, yy1;
glm::mat4 projection;

vector<int> indices;
vector<double> buffer;
vector<double> clippedLines;

double boundMin[2], boundMax[2];
double xmin, xmax, ymin, ymax;
double clipBoundary[10];
int nClippedLineVertices = 0;

void clipping()
{
	// clear old clipped lines
	clippedLines.clear();
	clippedLines.reserve(buffer.size());

	int nObjs = indices.size() - 1;
	for (int i = 0; i < nObjs; i++) {
		int begin = indices[i];
		int end = indices[i+1];

		for (int j = begin+1; j < end; j++) {
			int loc0 = (j-1) << 1;
			int loc1 = j << 1;
			
			// fetch a line segment
			double x0 = buffer[loc0];
			double y0 = buffer[loc0+1];
			double x1 = buffer[loc1];
			double y1 = buffer[loc1+1];

			// do clipping
			bool bAccept = line_clipping(x0, y0, x1, y1, xmin, ymin, xmax, ymax);
			if (bAccept) {
				clippedLines.push_back(x0);
				clippedLines.push_back(y0);
				clippedLines.push_back(x1);
				clippedLines.push_back(y1);
			}
		}
	}

	// setup VBO for clipped lines
	int bytesClippedLines = clippedLines.size() * sizeof(double);
	if (bytesClippedLines) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytesClippedLines, &clippedLines[0]);
		nClippedLineVertices = bytesClippedLines / (sizeof(double) * 2);
	}
}

void update_clipping_region()
{
	// calculate the clipping region via inverse projection
	glm::vec4 viewport(0.0, 0.0, (double)winWidth, (double)winHeight);
	glm::vec3 pmin = glm::unProject(glm::vec3((double)xx0, (double)yy0, 0.0),
		glm::mat4x4(), projection, viewport);
	glm::vec3 pmax = glm::unProject(glm::vec3((double)xx1, (double)yy1, 0.0),
		glm::mat4x4(), projection, viewport);

	// setting the clipping region
	xmin = pmin.x;
	ymin = pmin.y;
	xmax = pmax.x;
	ymax = pmax.y;
	if (xmin > xmax)
		swap(xmin, xmax);
	if (ymin > ymax)
		swap(ymin, ymax);

	// the vertices for the clipping boundary
	clipBoundary[0] = xmin;
	clipBoundary[1] = ymin;
	clipBoundary[2] = xmax;
	clipBoundary[3] = ymin;
	clipBoundary[4] = xmax;
	clipBoundary[5] = ymax;
	clipBoundary[6] = xmin;
	clipBoundary[7] = ymax;
	clipBoundary[8] = xmin;
	clipBoundary[9] = ymin;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		bLButtonDown = action == GLFW_PRESS;
		firstMouse = true;

		if (action == GLFW_RELEASE) {

			// do clipping here
			clipping();
		}
		break;
	default:
		break;
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	ypos = winHeight - ypos;

	if (bLButtonDown) {
		if (firstMouse) {
			xx0 = xpos;
			yy0 = ypos;
			firstMouse = false;
		}
		xx1 = xpos;
		yy1 = ypos;

		update_clipping_region();
	}
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	// set viewport with respect to current window size
	winWidth = width;
	winHeight = height;
	glViewport(0, 0, width, height);
}

bool load_geometry()
{
	// read the shape file to the buffers
	SHPHandle hShp = SHPOpen(shpPath, "rb");

	if (hShp->fpSHP == NULL)
		return false;

	int	nShapes = 0;
	SHPGetInfo(hShp, &nShapes, NULL, NULL, NULL);

	indices.reserve(hShp->nRecords << 1);
	buffer.reserve(hShp->nFileSize >> 4);

	int count = 0;
	for (int i = 0; i < nShapes; i++) {
		SHPObject *pObj = SHPReadObject(hShp, i);

		double *xx = pObj->padfX;
		double *yy = pObj->padfY;

		int nPoints = pObj->nVertices;
		int nParts = pObj->nParts;
		for (int j = 0; j < nParts; j++) {
			int begin = pObj->panPartStart[j];
			int end = j != nParts - 1 ? pObj->panPartStart[j + 1] : nPoints;

			indices.push_back(count);
			for (int k = begin; k < end; k++) {
				buffer.push_back(xx[k]);
				buffer.push_back(yy[k]);
				count++;
			}
		}

		SHPDestroyObject(pObj);
	}
	indices.push_back(count);

	// the bounding box for the objects
	memcpy(boundMin, hShp->adBoundsMin, 16);
	memcpy(boundMax, hShp->adBoundsMax, 16);

	SHPClose(hShp);
	
	return true;
}

void setup_vertex()
{
	// load shape files
	bool bLoad = load_geometry();
	if (!bLoad) {
		cerr << "File loading error!\n";
		glfwTerminate();
		exit(-1);
	}

	// setup the vertex buffer object
	glGenBuffers(nVBOs, VBO);
	glGenVertexArrays(nVBOs, VAO);

	int bufferSize = buffer.size();
	if (bufferSize > 0) {
		// setup original shape data
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(double)*bufferSize, &buffer[0], GL_STATIC_DRAW);
		glBindVertexArray(VAO[0]);
		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(double) * 2, NULL);
		glEnableVertexAttribArray(0);

		// setup buffer for clipped polylines
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(clipBoundary), NULL, GL_STATIC_DRAW);
		glBindVertexArray(VAO[1]);
		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(double)*2, NULL);
		glEnableVertexAttribArray(0);

		// setup buffer for clipping box
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(double)*bufferSize*2, NULL, GL_STATIC_DRAW);
		glBindVertexArray(VAO[2]);
		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(double)*2, NULL);
		glEnableVertexAttribArray(0);
	}
}

void destroy_vertex()
{
	glDeleteVertexArrays(nVBOs, VAO);
	glDeleteBuffers(nVBOs, VBO);
}

void setup_shader()
{
	pShader = new Shader("shader.vs", "shader.fs");
	// use the shader program
	pShader->use();

	// set model transform
	glm::mat4 model;
	pShader->setMat4("model", model);

	// set view transform
	glm::mat4 view;
	pShader->setMat4("view", view); // set uniform variable in the shader

}

void destroy_shader()
{
	if (pShader != NULL)
		delete pShader;
}

void setup_projection()
{
	double aspect = winHeight > 0 ? (double)winWidth / (double)winHeight : 1.0;
	double sceneWidth = boundMax[0] - boundMin[0];
	double sceneHeight = boundMax[1] - boundMin[1];
	if (sceneWidth >= sceneHeight) {
		double centerY = (boundMin[1] + boundMax[1])*0.5;
		double halfHeight = sceneWidth / aspect * 0.5;
		projection = glm::ortho(boundMin[0], boundMax[0], centerY - halfHeight, centerY + halfHeight);
	}
	else {
		double centerX = (boundMin[0] + boundMax[0])*0.5;
		double halfWidth = sceneHeight * aspect * 0.5;
		projection = glm::ortho(centerX - halfWidth, centerX + halfWidth, boundMin[1], boundMax[1]);
	}
	pShader->setMat4("projection", projection);
}

void render()
{
	// clear the frame buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// set projection
	setup_projection(); 

	// draw the map
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBindVertexArray(VAO[0]);
	pShader->setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	int nPolys = indices.size()-1;
	for (int i = 0; i < nPolys; i++) {
		int ind = indices[i];
		int nPoints = indices[i+1]-ind;

		glDrawArrays(GL_LINE_STRIP, ind, nPoints);
	}

	// draw the clipBoundary
	pShader->setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBindVertexArray(VAO[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(clipBoundary), clipBoundary);
	glDrawArrays(GL_LINE_STRIP, 0, sizeof(clipBoundary) / (sizeof(double) * 2));

	// draw clipped lines
	pShader->setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_LINES, 0, nClippedLineVertices);
}

int MainLoop()
{
	// initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// window creation
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "EX004", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// setup the callback functions
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	// load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// setup the vertex buffer and the shader
	setup_vertex();
	setup_shader();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// render
		render();

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy_shader();
	destroy_vertex();

	// terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();

	return 0;
}