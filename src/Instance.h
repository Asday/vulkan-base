#ifndef _e04e5c2e8e1549469cdb304bcc6d8b2a
#define _e04e5c2e8e1549469cdb304bcc6d8b2a

#include "Device.h"
#include "Loader.h"

#include <vulkan/vulkan.h>

#define INSTANCE_FUNCTIONS(o) \
	o(vkDestroyInstance) \
	o(vkEnumeratePhysicalDevices) \
	o(vkGetPhysicalDeviceProperties) \
	o(vkGetPhysicalDeviceQueueFamilyProperties) \
	o(vkCreateDevice)

class Instance {
	const Loader loader;
	public:
	VkInstance instance;

	Instance();  // Throws `std::runtime_error()`.
	Instance(const Instance&) = delete;
	Instance(Instance&&) = delete;
	Instance& operator=(const Instance&) = delete;
	Instance& operator=(Instance&&) = delete;
	~Instance() noexcept;

	#define o(name) PFN_##name name;
	INSTANCE_FUNCTIONS(o)
	#undef o

	Result<Device> createDevice() const noexcept;
};

#ifndef INSTANCE_IMPL
#undef INSTANCE_FUNCTIONS
#endif

#endif
