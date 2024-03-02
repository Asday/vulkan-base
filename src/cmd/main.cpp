#include "../DLL.h"
#include "../Device.h"
#include "../Loader.h"
#include "../Instance.h"

#include <vulkan/vulkan.h>

#include <string>

using namespace std::literals::string_literals;


int main() {
	const DLL dll{"libvulkan.so"s};
	const Loader loader{dll};
	const Instance instance{loader};
	const Device device{instance};
}
