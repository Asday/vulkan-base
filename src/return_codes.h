#ifndef _45a84d31fb5c471c8beac5b54334f5c9
#define _45a84d31fb5c471c8beac5b54334f5c9

#include <vulkan/vulkan.h>

#include <iostream>

std::ostream& operator<<(std::ostream& os, VkResult id);
bool success(VkResult r);

#endif
