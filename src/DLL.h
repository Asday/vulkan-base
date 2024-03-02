#ifndef _9ece23355a64419fbf3b0a8739c16dae
#define _9ece23355a64419fbf3b0a8739c16dae

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

class DLLNotFound: public std::runtime_error {
	public:
	DLLNotFound(const std::string& name) noexcept;
};

class DLL {
	void* dll;
	const std::string name;

	public:
	DLL(const std::string& name);  // throws `DLLNotFound`
	DLL(const DLL&) = delete;
	DLL(DLL&& other) noexcept;
	DLL& operator=(const DLL&) = delete;
	DLL& operator=(DLL&& other) noexcept;
	~DLL();

	friend std::ostream& operator<<(std::ostream& os, const DLL& dll);

	std::pair<std::optional<void*>, std::optional<std::string>>
	getHandle(const std::string& name) const noexcept;
};

#endif
