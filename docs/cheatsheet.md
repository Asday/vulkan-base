# Special methods / rule of 0/3/5.

Spelling                 | Meaning
-------------------------|-----------------
`T()`                    | constructor
`T(const T&)`            | copy constructor
`T(T&&)`                 | move constructor
`T& operator=(const T&)` | copy assigner
`T& operator=(T&&)`      | move assigner
`~T()`                   | destructor

Make everything noexcept except for constructors and functions used in constructors.

# `.String()` but in C++

```cpp
std::ostream& operator<<(std::ostream& os, T t) {
	os << t.somePublicData;
	return os;
}

// Or:

class T {
	// ...
	friend std::ostream& operator<<(std::ostream& os, const T& t) {
		os << t.somePrivateData;
		return os;
	}
}
```

# Enum nonsense

C++ is kinda sucky with this so you HAVE to use the preprocessor.

```cpp
#define ENUM_IDENTIFIERS(o) \
	o(firstID) \
	o(secondID) \
	o(thirdID)

#define o(id) id,
enum EnumName { ENUM_IDENTIFIERS(o) };
#undef o

#define o(id) case id: os << #id##s; break;
std::ostream& operator<<(std::ostream& os, EnumName id) {
	switch (id) {
		ENUM_IDENTIFIERS(o)
		default: os.setstate(std::ios_base::failbit);
	}
	return os;
}

#undef ENUM_IDENTIFIERS
```
