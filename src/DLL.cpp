#include "DLL.h"

#include <dlfcn.h>
#include <optional>
#include <stdexcept>
#include <sstream>
#include <string>
#include <utility>

using namespace std::literals::string_literals;

DLLNotFound::DLLNotFound(const std::string& name) noexcept:
	std::runtime_error(
		(std::stringstream() << "missing DLL `"s << name << "`"s).str()
	) {}

DLL::DLL(const std::string& name) :
	dll([name]{
		void* dll = dlopen(name.c_str(), RTLD_NOW);
		if (dll == nullptr) throw DLLNotFound(name);
		return dll;
	}()),
	name(name) {}

DLL::DLL(DLL&& other) noexcept:
	dll(std::exchange(other.dll, nullptr)) {}

DLL& DLL::operator=(DLL&& other) noexcept {
	std::swap(dll, other.dll);

	return *this;
}

std::ostream& operator<<(std::ostream& os, const DLL& dll) {
	os << dll.name;

	return os;
}

std::pair<std::optional<void*>, std::optional<std::string>>
DLL::getHandle(const std::string& name) const noexcept {
	dlerror();  // Clear any existing errors.
	void* ret{dlsym(dll, name.c_str())};

	char* error{dlerror()};
	if (error != NULL) return std::make_pair(std::nullopt, std::string{error});

	return std::make_pair(ret, std::nullopt);
}

DLL::~DLL() {
	dlclose(dll);
}
