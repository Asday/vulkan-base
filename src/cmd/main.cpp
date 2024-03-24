#include "../GLFW.h"
#include "../Instance.h"

#include <GLFW/glfw3.h>

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
