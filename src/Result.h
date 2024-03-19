#ifndef _b217bc42e40a444dbfb9bcc25f94b9b3
#define _b217bc42e40a444dbfb9bcc25f94b9b3

#include <string>

template <typename T>
struct Result {
	bool success;
	union {
		T value;
		std::string error;
	};

	~Result() noexcept { value.~T(); };
};

#endif
