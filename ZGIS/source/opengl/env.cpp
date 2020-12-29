#include "env.h"
#include "model/GeoMap.h"

namespace Env {

Shader pointShader;
Shader lineShader;
Shader polygonShader;
Shader borderShader;
Shader highlightShader;
Shader textureShader;
Renderer renderer;

void createShaders() {
    printf("point");
    pointShader.create("source/opengl/shaders/point_vert.glsl", "source/opengl/shaders/point_frag.glsl");
    printf("line");
    lineShader.create("source/opengl/shaders/line_vert.glsl", "source/opengl/shaders/line_frag.glsl");
    printf("polygon");
    polygonShader.create("source/opengl/shaders/polygon_vert.glsl", "source/opengl/shaders/polygon_frag.glsl");
    printf("border");
    borderShader.create("source/opengl/shaders/border_vert.glsl", "source/opengl/shaders/border_frag.glsl");
    printf("hightlight");
    highlightShader.create("source/opengl/shaders/highlight_vert.glsl", "source/opengl/shaders/highlight_frag.glsl");
    printf("texture");
    textureShader.create("source/opengl/shaders/texture_vert.glsl", "source/opengl/shaders/texture_frag.glsl");
}

bool isEditing = false;
CursorType cursorType = CursorType::Normal;
GeoMap* map = new GeoMap();
//OperationList opList;
QString HOME = ".";


} // namespace Env
