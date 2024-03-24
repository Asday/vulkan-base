#ifndef _7aaaacfeb5b247c79b61fb5c9da6963e
#define _7aaaacfeb5b247c79b61fb5c9da6963e

#include <GLFW/glfw3.h>

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
