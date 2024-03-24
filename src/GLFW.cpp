#include "GLFW.h"

GLFW::GLFW() noexcept { glfwInit(); }
GLFW::~GLFW() noexcept { glfwTerminate(); }

Window GLFW::createWindow() const noexcept { return {}; }
