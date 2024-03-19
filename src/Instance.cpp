#define INSTANCE_IMPL
#include "Instance.h"
#undef INSTANCE_IMPL

#include "Loader.h"
#include "return_codes.h"

#include <stdexcept>
#include <sstream>
#include <string>

using namespace std::literals::string_literals;

Instance::Instance() : loader(Loader()) {
	VkApplicationInfo applicationInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = "API without Secrets: Introduction to Vulkan",
		.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
		.pEngineName = "Vulkan Tutorial by Intel",
		.engineVersion = VK_MAKE_VERSION( 1, 0, 0 ),
		.apiVersion = VK_MAKE_VERSION( 1, 0, 0 ),
	};

	VkInstanceCreateInfo instanceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.pApplicationInfo = &applicationInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = 0,
		.ppEnabledExtensionNames = nullptr,
	};

	{
		const VkResult r = loader.vkCreateInstance(
			&instanceCreateInfo,
			nullptr,
			&instance
		);
		if (!success(r)) {
			throw std::runtime_error(
				(std::stringstream() << "failed to create instance: "s << r).str()
			);
		}
	}

	#define o(name) \
		name = reinterpret_cast<PFN_##name>( \
			loader.vkGetInstanceProcAddr(instance, #name) \
		);
	INSTANCE_FUNCTIONS(o)
	#undef o
	#undef INSTANCE_FUNCTIONS
}

Instance::~Instance() { vkDestroyInstance(instance, nullptr); }

Result<Device> Instance::createDevice() const noexcept {
	try {
		return {.success{true}, .value{*this}};
	} catch (const std::runtime_error& e) {
		return {.success{false}, .error{e.what()}};
	}
}
