#ifndef _d87578f7c85f4c0e92293d5215231652
#define _d87578f7c85f4c0e92293d5215231652

#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

class GLFW {
	public:
	GLFW() noexcept;
	GLFW(const GLFW&) = delete;
	GLFW(GLFW&&) = delete;
	GLFW& operator=(const GLFW&) = delete;
	GLFW& operator=(GLFW&&) = delete;
	~GLFW() noexcept;

	Window createWindow() const noexcept;
};

#endif
