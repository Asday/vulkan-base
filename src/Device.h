#ifndef _da28592f06164ab3b523a74ee2325ff3
#define _da28592f06164ab3b523a74ee2325ff3

#include <vulkan/vulkan.h>

class Instance;

#define DEVICE_FUNCTIONS(o) \
	o(vkGetDeviceQueue) \
	o(vkDestroyDevice)

class Device {
	VkDevice device;
	VkQueue queue;

	public:
	Device(const Instance& instance);  // Throws `std::runtime_error()`.
	Device(const Device&) = delete;
	Device(Device&&) = delete;
	Device& operator=(const Device&) = delete;
	Device& operator=(Device&&) = delete;
	~Device();

	#define o(name) PFN_##name name;
	DEVICE_FUNCTIONS(o)
	#undef o
};

#ifndef DEVICE_IMPL
#undef DEVICE_FUNCTIONS
#endif

#endif
