#define DEVICE_IMPL
#include "Device.h"
#undef DEVICE_IMPL

#include "Instance.h"

#include "return_codes.h"
#include "narrow_cast.h"

#include <stdexcept>
#include <sstream>
#include <vector>

using namespace std::literals::string_literals;

namespace {
	uint32_t countDevices(const Instance& instance) {
		uint32_t deviceCount;
		const VkResult r{instance.vkEnumeratePhysicalDevices(
			instance.instance,
			&deviceCount,
			nullptr
		)};
		if (!success(r)) {
			throw std::runtime_error(
				(std::stringstream() << "failed to count physical devices: "s << r)
				.str()
			);
		}

		return deviceCount;
	}

	std::vector<VkPhysicalDevice> enumerateDevices(const Instance& instance) {
		uint32_t deviceCount{countDevices(instance)};

		auto devices{std::vector<VkPhysicalDevice>(deviceCount)};
		const VkResult r{instance.vkEnumeratePhysicalDevices(
			instance.instance,
			&deviceCount,
			devices.data()
		)};
		if (!success(r)) {
			throw std::runtime_error((
				std::stringstream() << "failed to enumerate physical devices: "s << r
			).str());
		}

		return devices;
	}

	bool isSuitable(const Instance& instance, const VkPhysicalDevice& d) {
		VkPhysicalDeviceProperties p;
		instance.vkGetPhysicalDeviceProperties(d, &p);
		return (
			VK_VERSION_MAJOR(p.apiVersion) >= 1
			|| p.limits.maxImageDimension2D >= 4096
		);
	}

	uint32_t
	countQueueFamilies(const Instance& instance, const VkPhysicalDevice& d) {
		uint32_t c;
		instance.vkGetPhysicalDeviceQueueFamilyProperties(d, &c, nullptr);

		return c;
	}

	std::vector<VkQueueFamilyProperties> enumerateQueueFamilies(
		const Instance& instance,
		const VkPhysicalDevice& d
	) {
		uint32_t c{countQueueFamilies(instance, d)};
		auto qfs{std::vector<VkQueueFamilyProperties>(c)};
		instance.vkGetPhysicalDeviceQueueFamilyProperties(d, &c, qfs.data());

		return qfs;
	}

	bool isSuitable(const VkQueueFamilyProperties& qf) {
		return (qf.queueCount > 0) && (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT);
	}

	std::optional<uint32_t> findSuitableQueueFamilyIndex(
		const Instance& instance,
		const VkPhysicalDevice& d
	) {
		auto queueFamilies{enumerateQueueFamilies(instance, d)};
		for (uint32_t i = 0; i < queueFamilies.size(); i++) {
			const auto& qf{queueFamilies[i]};
			if (isSuitable(qf)) return {i};
		}

		return std::nullopt;
	}

	VkDevice createDevice(
		const Instance& instance,
		const VkPhysicalDevice& d,
		const uint32_t sqfi
	) {
		std::vector<float> queuePriorities{1.0f};
		VkDeviceQueueCreateInfo queueCreateInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = sqfi,
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

		VkDevice device;
		const VkResult r{instance.vkCreateDevice(
			d,
			&deviceCreateInfo,
			nullptr,
			&device
		)};
		if (!success(r)) {
			throw std::runtime_error(
				(std::stringstream() << "failed to create logical device: "s << r).str()
			);
		}

		return device;
	}

	VkDevice findAndCreateDevice(const Instance& instance) {
		for (const auto& d : enumerateDevices(instance)) {
			if (isSuitable(instance, d)) {
				auto sqfi{findSuitableQueueFamilyIndex(instance, d)};
				if (sqfi) {
					return createDevice(instance, d, sqfi.value());
				}
			}
		}

		throw std::runtime_error("unable to find a suitable device"s);
	}
}

Device::Device(const Instance& instance) :
	device(findAndCreateDevice(instance)) {
	#define o(name) \
		name = reinterpret_cast<PFN_##name>( \
			instance.vkGetDeviceProcAddr(device, #name) \
		);
	DEVICE_FUNCTIONS(o)
	#undef o
	#undef DEVICE_FUNCTIONS
}

Device::~Device() { vkDestroyDevice(device, nullptr); }
