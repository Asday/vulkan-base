#include "../Device.h"
#include "../Instance.h"

#include <vulkan/vulkan.h>

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
}
