#ifndef _da28592f06164ab3b523a74ee2325ff3
#define _da28592f06164ab3b523a74ee2325ff3

#include <vulkan/vulkan.h>

class Instance;

class Device {
	VkDevice device;

	public:
	Device(const Instance& instance);  // Throws `std::runtime_error()`.
	Device(const Device&) = delete;
	Device(Device&&) = delete;
	Device& operator=(const Device&) = delete;
	Device& operator=(Device&&) = delete;
	~Device();
};

#endif
