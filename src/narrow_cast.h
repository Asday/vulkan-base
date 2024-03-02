#ifndef _3cdb1772dd0949f480d5336437710031
#define _3cdb1772dd0949f480d5336437710031

#include <stdexcept>
#include <string>

using namespace std::literals::string_literals;

class InformationLoss : public std::runtime_error {
	public:
	InformationLoss() noexcept:
		std::runtime_error("information loss"s) {}
};

template <typename T, typename U>
constexpr T narrow_cast(const U&& u) {  // Throws `InformationLoss()`.
	const T o{static_cast<T>(u)};

	if (!(static_cast<U>(o) == u)) throw InformationLoss();

	return o;
}

#endif
