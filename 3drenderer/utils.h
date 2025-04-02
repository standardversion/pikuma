#pragma once
#include <vector>
#include <string>

namespace utils
{
	std::vector<std::string> split(const std::string& str, const std::string& delimeter);
	double lerp(double a, double b, double t);
}
