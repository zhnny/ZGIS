#pragma once
#ifndef RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "shapefil.h"
#include "LineClipping.h"

int mainLoop(int width, int height, std::string windowName);
void setCommand(std::string cmd);
void setModel(float scale = 1.0);

#endif // !RENDER_H
