#pragma once

#include <string>
#include <vector>

// String splitting utility function inspired by Python's str.split()
std::vector<std::string> split(const std::string& input, const char* delimiter) {
	// Note that std::getline does not work if the delimiter consists of more than a single character
	std::vector<std::string> output;
	bool escape = false;
	int previous = 0;
	while (!escape) {
		int current = input.find(delimiter, previous);
		if (current == -1) {
			current = input.size();
			escape = true;
		}
		output.emplace_back(input.substr(previous, current - previous));
		previous = current + strlen(delimiter);
	}
	return output;
}

std::vector<std::string> split(const std::string& input, const std::string& delimiter) {
	return split(input, delimiter.c_str());
}