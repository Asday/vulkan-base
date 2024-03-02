#define VULKAN_IMPL
#include "Loader.h"
#undef VULKAN_IMPL

#include "DLL.h"

#include <stdexcept>

Loader::Loader(const DLL& dll) {
	const auto r{dll.getHandle("vkGetInstanceProcAddr")};
	if (!r.first) throw std::runtime_error(r.second.value());

	vkGetInstanceProcAddr =
		reinterpret_cast<PFN_vkGetInstanceProcAddr>(r.first.value());

	#define o(name) \
		name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(nullptr, #name));
	GLOBAL_FUNCTIONS(o)
	#undef o
	#undef GLOBAL_FUNCTIONS
}
