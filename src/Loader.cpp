#define LOADER_IMPL
#include "Loader.h"
#undef LOADER_IMPL

#include "DLL.h"

#include <stdexcept>
#include <string>

using namespace std::literals::string_literals;

Loader::Loader() : dll(DLL("libvulkan.so"s)) {
	const auto r{dll.getHandle("vkGetInstanceProcAddr")};
	if (!r.success) throw std::runtime_error(r.error);

	vkGetInstanceProcAddr =
		reinterpret_cast<PFN_vkGetInstanceProcAddr>(r.value);

	#define o(name) \
		name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(nullptr, #name));
	GLOBAL_FUNCTIONS(o)
	#undef o
	#undef GLOBAL_FUNCTIONS
}
