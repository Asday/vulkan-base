#ifndef _7aaaacfeb5b247c79b61fb5c9da6963e
#define _7aaaacfeb5b247c79b61fb5c9da6963e

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

class GLFW;

class Window {
	public:
	GLFWwindow* window;
	Window() noexcept;
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
	~Window() noexcept;

};

#endif
