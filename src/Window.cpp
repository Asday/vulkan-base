#include "Window.h"

#include "GLFW.h"

namespace {
	GLFWwindow* createWindow() {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		return glfwCreateWindow(1280, 720, "Vulkan", nullptr, nullptr);
	}
}

Window::Window() noexcept : window(createWindow()) {}

Window::~Window() noexcept { glfwDestroyWindow(window); }
