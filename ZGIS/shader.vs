#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 aColor;
out vec3 color;

void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
	color = aColor;
}
