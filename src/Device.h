#ifndef _da28592f06164ab3b523a74ee2325ff3
#define _da28592f06164ab3b523a74ee2325ff3

#include <vulkan/vulkan.h>

#include "Instance.h"

class Device {
	VkDevice device;

	public:
	Device(const Instance& instance);  // Throws `std::runtime_error()`.
	Device(const Device&);
	Device(Device&&);
	Device& operator=(const Device&);
	Device& operator=(Device&&);
	~Device();
};

#endif
