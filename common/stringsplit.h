#pragma once

#include <string>
#include <vector>

// String splitting utility function inspired by Python's str.split()
// However, there is a major difference:
// if the delimiter is empty, instead of throwing an exception,
// a vector containing only the intact string will be returned
[[nodiscard]] std::vector<std::string> split(const std::string& input, const char* delimiter) {
	// Note that std::getline does not work if the delimiter consists of more than a single character
	std::vector<std::string> output;
	if (input == "") return output;
	if (delimiter == "") {
		output.emplace_back(input);
		return output;
	}
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

[[nodiscard]] std::vector<std::string> split(const std::string& input, const std::string& delimiter) {
	return split(input, delimiter.c_str());
}


// Strip is also included here since sometimes bad things happen if it's not used due to rogue whitespace
[[nodiscard]] std::string strip(const std::string& input, const std::string& to_strip = " \n\t\f\r\v") {
	if (input == "") return input;
	
	size_t first_pos = 0;
	for (const auto& c : input) {
		bool broke_loop = false;
		for (const auto& c_strip : to_strip) {
			if (c == c_strip) {
				++first_pos;
				broke_loop = true;
				break;
			}
		}
		if (!broke_loop) break;
	}
	std::string output = input.substr(first_pos);

	int end_char = static_cast<int>(output.size()) - 1;
	for (int i = end_char; i >= 0; --i) {
		bool found_something = false;
		for (const auto& c_strip : to_strip) {
			if (output[i] == c_strip) {
				found_something = true;
				break;
			}
		}
		if (!found_something) {
			end_char = i;
			break;
		}
	}

	output = output.substr(0, static_cast<size_t>(end_char) + 1);
	return output;
}