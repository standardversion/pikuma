#pragma once
#include <vector>
#include <string>

namespace utils
{
	std::vector<std::string> split(const std::string& str, const std::string& delimeter);
	void int_swap(int* a, int* b);
}
