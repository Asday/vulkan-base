#define INSTANCE_IMPL
#include "Instance.h"
#undef INSTANCE_IMPL

#include "Loader.h"
#include "return_codes.h"

#include <array>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

using namespace std::literals;
using namespace std::literals::string_literals;

#ifdef NDEBUG
	constexpr std::array<std::string_view, 0> wantedLayers{};
#else
	#define WANTED_LAYERS(o) o(VK_LAYER_KHRONOS_validation)
	#define o(name) #name##sv
	constexpr std::array wantedLayers{
		WANTED_LAYERS(o),
	};
	#undef o
	#define o(name) #name
	constexpr std::array wantedLayersCStrs{
		WANTED_LAYERS(o),
	};
	#undef o
	#undef WANTED_LAYERS
#endif

namespace {
	uint32_t countLayerProperties(const Loader& loader) {
		uint32_t c;
		loader.vkEnumerateInstanceLayerProperties(&c, nullptr);

		return c;
	}

	std::vector<VkLayerProperties>
	enumerateLayerProperties(const Loader& loader) {
		uint32_t c{countLayerProperties(loader)};
		auto lps{std::vector<VkLayerProperties>(c)};

		loader.vkEnumerateInstanceLayerProperties(&c, lps.data());

		return lps;
	}

	std::vector<std::string_view> missingWantedLayers(const Loader& loader) {
		std::vector<std::string_view> missing{};
		{
			std::vector<std::string_view> layerNames;
			{
				std::vector<VkLayerProperties> lps{enumerateLayerProperties(loader)};
				layerNames = std::vector<std::string_view>(lps.size());
				for (const auto& lp : lps) { layerNames.push_back(lp.layerName); }
			}

			for (const auto& wantedLayer : wantedLayers) {
				for (const auto& gotLayer : layerNames) {
					// Can't `std::find()` for some reason.
					// See: `docs/find-in-vector-of-string-views.md`
					if (gotLayer == wantedLayer) { goto found; }
				}
				missing.push_back(wantedLayer);
				found:
			}
		}

		return missing;
	}
}

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

	if constexpr(!wantedLayers.empty()) {
		if (auto missing{missingWantedLayers(loader)}; !missing.empty()) {
			std::stringstream error;
			error << "failed to find layer(s): "s;
			for (const auto& layer : missing) { error << layer; }

			throw std::runtime_error(error.str());
		}

		instanceCreateInfo.enabledLayerCount = wantedLayers.size();
		instanceCreateInfo.ppEnabledLayerNames = wantedLayersCStrs.data();
	}

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
