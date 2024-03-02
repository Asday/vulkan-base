#include "../DLL.h"
#include "../Loader.h"
#include "../Instance.h"
#include "../narrow_cast.h"
#include "../return_codes.h"

#include <vulkan/vulkan.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std::literals::string_literals;


int main() {
	const DLL dll{"libvulkan.so"s};
	const Loader loader{dll};
	const Instance instance{loader};

	VkDevice device{};
	{
		uint32_t deviceCount{};
		{
			const VkResult r = instance.vkEnumeratePhysicalDevices(
				instance.instance,
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
			const VkResult r = instance.vkEnumeratePhysicalDevices(
				instance.instance,
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
			instance.vkGetPhysicalDeviceProperties(d, &properties);
			if (
				VK_VERSION_MAJOR(properties.apiVersion) >= 1
				|| properties.limits.maxImageDimension2D >= 4096
			) {
				uint32_t queueFamilyCount{};
				instance.vkGetPhysicalDeviceQueueFamilyProperties(
					d,
					&queueFamilyCount,
					nullptr
				);
				auto queueFamilies{
					std::vector<VkQueueFamilyProperties>(queueFamilyCount)
				};
				instance.vkGetPhysicalDeviceQueueFamilyProperties(
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
							const VkResult r = instance.vkCreateDevice(
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
