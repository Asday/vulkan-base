#include "../GLFW.h"
#include "../Instance.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include <iostream>
#include <string>

using namespace std::literals::string_literals;


int main() {
	const Instance instance{};
	const auto r{instance.createDevice()};
	if (!r.success) {
		std::cout << "failed to create a device: "s << r.error << std::endl;
		return -1;
	}

	const GLFW glfw{};
	const auto window{glfw.createWindow()};
	while (!glfwWindowShouldClose(window.window)) {
		glfwPollEvents();
	}
}
