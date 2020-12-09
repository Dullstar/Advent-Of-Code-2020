#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "stringsplit.h"

std::vector<std::string> parse_input(const std::string& filename) {
	std::ifstream file(filename);
	std::stringstream sstream;
	sstream << file.rdbuf();
	auto groups = split(strip(sstream.str()), "\n\n");
	return groups;
}

int get_unique_answers(const std::string& group) {
	std::unordered_map<char, bool> q_map;

	for (const char& c : group) {
		if (isalpha(c)) q_map[c] = true;
	}
	return std::count_if(q_map.begin(), q_map.end(), [](auto a) { return a.second == true; });
}

int get_shared_answers(const std::string& group) {
	std::unordered_map<char, int> q_map;
	for (const char& c : "abcdefghijklmnopqrstuvwxyz")
		q_map[c] = 0;
	auto individuals = split(group, "\n");
	for (const auto& individual : individuals)
		for (const char& answer : individual) {
			if (isalpha(answer)) q_map[answer]++;
		}
	return std::count_if(q_map.begin(), q_map.end(), [&](auto a) { return static_cast<int>(individuals.size()) == a.second; });
}

int main() {
	auto groups = parse_input("input.txt");
	int unique_answers = 0;
	int shared_answers = 0;

	for (const auto& group : groups) {
		unique_answers += get_unique_answers(group);
		shared_answers += get_shared_answers(group);
	}

	// I think this output could probably be worded better
	std::cout << "Unique answers per group " << unique_answers << "\n";
	std::cout << "Shared answers per group " << shared_answers << "\n";
}