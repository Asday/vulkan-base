#ifndef _5b7ce91669074f7aaffb2cef8e14d531
#define _5b7ce91669074f7aaffb2cef8e14d531

#include "DLL.h"

#include <vulkan/vulkan.h>

#define EXPORTED_FUNCTIONS(o) o(vkGetInstanceProcAddr)
#define GLOBAL_FUNCTIONS(o) \
	o(vkCreateInstance) \
	o(vkEnumerateInstanceLayerProperties)

class Loader {
	const DLL dll;
	public:
	Loader();  // Throws `std::runtime_error()`.

	#define o(name) PFN_##name name;
	EXPORTED_FUNCTIONS(o)
	GLOBAL_FUNCTIONS(o)
	#undef o
	#undef EXPORTED_FUNCTIONS
};

#ifndef LOADER_IMPL
#undef GLOBAL_FUNCTIONS
#endif

#endif
