#include "../DLL.h"
#include "../narrow_cast.h"

#include <vulkan/vulkan.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std::literals::string_literals;

#define SUCCESS_CODES(o) \
	o(VK_SUCCESS) \
	o(VK_NOT_READY) \
	o(VK_TIMEOUT) \
	o(VK_EVENT_SET) \
	o(VK_EVENT_RESET) \
	o(VK_INCOMPLETE) \
	o(VK_SUBOPTIMAL_KHR) \
	o(VK_THREAD_IDLE_KHR) \
	o(VK_THREAD_DONE_KHR) \
	o(VK_OPERATION_DEFERRED_KHR) \
	o(VK_OPERATION_NOT_DEFERRED_KHR) \
	o(VK_PIPELINE_COMPILE_REQUIRED) \
	o(VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT)

#define ERROR_CODES(o) \
	o(VK_ERROR_OUT_OF_HOST_MEMORY) \
	o(VK_ERROR_OUT_OF_DEVICE_MEMORY) \
	o(VK_ERROR_INITIALIZATION_FAILED) \
	o(VK_ERROR_DEVICE_LOST) \
	o(VK_ERROR_MEMORY_MAP_FAILED) \
	o(VK_ERROR_LAYER_NOT_PRESENT) \
	o(VK_ERROR_EXTENSION_NOT_PRESENT) \
	o(VK_ERROR_FEATURE_NOT_PRESENT) \
	o(VK_ERROR_INCOMPATIBLE_DRIVER) \
	o(VK_ERROR_TOO_MANY_OBJECTS) \
	o(VK_ERROR_FORMAT_NOT_SUPPORTED) \
	o(VK_ERROR_FRAGMENTED_POOL) \
	o(VK_ERROR_SURFACE_LOST_KHR) \
	o(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR) \
	o(VK_ERROR_OUT_OF_DATE_KHR) \
	o(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR) \
	o(VK_ERROR_INVALID_SHADER_NV) \
	o(VK_ERROR_OUT_OF_POOL_MEMORY) \
	o(VK_ERROR_INVALID_EXTERNAL_HANDLE) \
	o(VK_ERROR_FRAGMENTATION) \
	o(VK_ERROR_INVALID_DEVICE_ADDRESS_EXT) \
	o(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT) \
	o(VK_ERROR_VALIDATION_FAILED_EXT) \
	o(VK_ERROR_COMPRESSION_EXHAUSTED_EXT) \
	o(VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR) \
	o(VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR) \
	o(VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR) \
	o(VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR) \
	o(VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR) \
	o(VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR) \
	o(VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR) \
	o(VK_ERROR_UNKNOWN)

#define o(id) case id: os << #id##s; break;
std::ostream& operator<<(std::ostream& os, VkResult id) {
	switch (id) {
		SUCCESS_CODES(o)
		ERROR_CODES(o)
		default: os.setstate(std::ios_base::failbit);
	}
	return os;
}
#undef o

#undef SUCCESS_CODES
#undef ERROR_CODES

bool success(VkResult r) { return r >= 0; };

int main() {
	const DLL vulkan{"libvulkan.so"s};

	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	{
		const auto r{vulkan.getHandle("vkGetInstanceProcAddr")};
		if (!r.first) {
			std::cout << r.second.value() << std::endl;
			return 1;
		} else {
			vkGetInstanceProcAddr =
				reinterpret_cast<PFN_vkGetInstanceProcAddr>(r.first.value());
		}
	}

	#define GLOBAL_FUNCTIONS(o) \
		o(vkCreateInstance)

	#define o(name) \
		auto name{ \
			reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(nullptr, #name)) \
		};
	GLOBAL_FUNCTIONS(o)
	#undef o
	#undef GLOBAL_FUNCTIONS

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

	VkInstance vkInstance;
	{
		const VkResult r = vkCreateInstance(
			&instanceCreateInfo,
			nullptr,
			&vkInstance
		);
		if (!success(r)) {
			std::cout << "failed to create instance: "s << r << std::endl;
			return 2;
		}
	}

	#define INSTANCE_FUNCTIONS(o) \
		o(vkEnumeratePhysicalDevices) \
		o(vkGetPhysicalDeviceProperties) \
		o(vkGetPhysicalDeviceQueueFamilyProperties) \
		o(vkCreateDevice)

	#define o(name) \
		auto name{ \
			reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(vkInstance, #name)) \
		};

	INSTANCE_FUNCTIONS(o)
	#undef o
	#undef INSTANCE_FUNCTIONS

	VkDevice device{};
	{
		uint32_t deviceCount{};
		{
			const VkResult r = vkEnumeratePhysicalDevices(
				vkInstance,
				&deviceCount,
				nullptr
			);
			if (!success(r)) {
				std::cout << "failed to count physical devices: "s << r << std::endl;
				return 3;
			}
		}
		auto devices{std::vector<VkPhysicalDevice>(deviceCount)};
		{
			const VkResult r = vkEnumeratePhysicalDevices(
				vkInstance,
				&deviceCount,
				devices.data()
			);
			if (!success(r)) {
				std::cout
					<< "failed to enumerate physical devices: "s
					<< r
					<< std::endl;
				return 4;
			}
		}

		for (const auto& d : devices) {
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(d, &properties);
			if (
				VK_VERSION_MAJOR(properties.apiVersion) >= 1
				|| properties.limits.maxImageDimension2D >= 4096
			) {
				uint32_t queueFamilyCount{};
				vkGetPhysicalDeviceQueueFamilyProperties(d, &queueFamilyCount, nullptr);
				auto queueFamilies{
					std::vector<VkQueueFamilyProperties>(queueFamilyCount)
				};
				vkGetPhysicalDeviceQueueFamilyProperties(
					d,
					&queueFamilyCount,
					queueFamilies.data()
				);
				for (uint32_t i = 0; i < queueFamilies.size(); i++) {
					const auto& qf = queueFamilies[i];
					if (qf.queueCount > 0 && qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						std::vector<float> queuePriorities{1.0f};
						VkDeviceQueueCreateInfo queueCreateInfo{
							.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
							.pNext = nullptr,
							.flags = 0,
							.queueFamilyIndex = i,
							.queueCount = narrow_cast<uint32_t>(queuePriorities.size()),
							.pQueuePriorities = queuePriorities.data(),
						};
						VkDeviceCreateInfo deviceCreateInfo{
							.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
							.pNext = nullptr,
							.flags = 0,
							.queueCreateInfoCount = 1,
							.pQueueCreateInfos = &queueCreateInfo,
							.enabledLayerCount = 0,
							.ppEnabledLayerNames = nullptr,
							.enabledExtensionCount = 0,
							.ppEnabledExtensionNames = nullptr,
							.pEnabledFeatures = nullptr,
						};
						{
							const VkResult r = vkCreateDevice(
								d,
								&deviceCreateInfo,
								nullptr,
								&device
							);
							if (!success(r)) {
								std::cout
									<< "failed to create logical device: "
									<< r
									<< std::endl;
								return 6;
							}
						}
						goto choseDevice;
					}
				}
			}
		}
	}
	std::cout << "unable to find a suitable device"s << std::endl;
	return 5;
	choseDevice:
}
