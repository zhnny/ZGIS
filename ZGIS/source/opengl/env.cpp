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
    pointShader.create("shaders/point_vert.glsl", "shaders/point_frag.glsl");
    printf("line");
    lineShader.create("shaders/line_vert.glsl", "shaders/line_frag.glsl");
    printf("polygon");
    polygonShader.create("shaders/polygon_vert.glsl", "shaders/polygon_frag.glsl");
    printf("border");
    borderShader.create("shaders/border_vert.glsl", "shaders/border_frag.glsl");
    printf("hightlight");
    highlightShader.create("shaders/highlight_vert.glsl", "shaders/highlight_frag.glsl");
    printf("texture");
    textureShader.create("shaders/texture_vert.glsl", "shaders/texture_frag.glsl");
}

bool isEditing = false;
CursorType cursorType = CursorType::Normal;
GeoMap* map = new GeoMap();
//OperationList opList;
QString HOME = ".";


} // namespace Env
