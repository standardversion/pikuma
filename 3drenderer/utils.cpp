#include "utils.h"

namespace utils
{
	std::vector<std::string> split(const std::string& str, const std::string& delimeter)
	{
		std::vector<std::string> items;
		std::size_t start{ 0 };
		for (size_t found{ str.find(delimeter) }; found != std::string::npos; found = str.find(delimeter, start))
		{
			items.emplace_back(str.begin() + start, str.begin() + found);
			start = found + delimeter.size();
		}
		if (start != str.size())
		{
			items.emplace_back(str.begin() + start, str.end());
		}
		return items;
	}
}