#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "timer.h"
#include "stringsplit.h"

class Range {
public:
	int min;
	int max;
	Range(int _min, int _max)
		: min(_min), max(_max) {
		if (min > max) {
			std::swap(min, max);
		}
	}

	bool is_in_range(int x) const {
		if (x > max || x < min)
			return false;
		return true;
	}
};


// I can't think of a good class name today.
class InputContents {
public:
	static InputContents parse_input(const char* filename);
	int scanning_error_rate() {
		Timer timer;
		int total = 0;
		for (const auto& ticket : nearby_tickets) {
			bool ticket_valid = true;
			for (const int& number : ticket) {
				if (!is_number_valid(number)) {
					total += number;
					ticket_valid = false;
					// We could stop checking here, if and only if a ticket contains at most one invalid entry
					// but while that experimentally seems to be the case, the directions don't appear to explicitly
					// tell us that's a safe assumption.
				}
			}
			if (ticket_valid) valid_nearby_tickets.push_back(ticket);  // copy paste
		}
		return total;
	}

	long long departure_fields_product() {
		Timer timer;
		std::vector<std::pair<std::vector<int>, std::string>> pairs;
		for (const auto& field : fields) {
			std::vector<int> possible;
			for (size_t i = 0; i < my_ticket.size(); i++) {
				bool passes = true;
				for (const std::vector<int>& ticket : valid_nearby_tickets) {
					bool in_range = false;
					for (const Range& range : field.second) {
						if (range.is_in_range(ticket[i])) {
							in_range = true;
							break;
						}
					}
					if (!in_range) {
						passes = false;
						break;
					}
				}
				if (passes) {
					// std::cout << "Field: " << field.first << " passes all tickets for index " << i << "\n";
					possible.push_back(i);
				}
			}
			pairs.emplace_back(std::pair<std::vector<int>, std::string>(possible, field.first));
		}

		std::sort(pairs.begin(), pairs.end(), [&](auto& a, auto& b) {
			return a.first.size() < b.first.size();
			});

		std::unordered_map<std::string, int> field_and_index;
		std::vector<bool> taken(my_ticket.size(), false);
		for (const auto& pair : pairs) {
			for (const int& index : pair.first) {
				if (!taken[index]) {
					field_and_index[pair.second] = index;
					taken[index] = true;
				}
			}
		}

		long long result = 1;
		for (const auto& field : field_and_index) {
			std::cout << field.first << " -> " << my_ticket[field.second] << "\n";
			auto departure_string = field.first.find("departure");
			if (departure_string != std::string::npos) {
				result *= my_ticket[field.second];
			}
		}
		return result;
	}

private:
	InputContents() = default;
	std::unordered_map<std::string, std::vector<Range>> fields;
	std::vector<int> my_ticket;
	std::vector<std::vector<int>> nearby_tickets;
	std::vector<bool> allowable_values;
	std::vector<std::vector<int>> valid_nearby_tickets;

	void get_allowable_values(int max_range) {
		allowable_values = std::vector<bool>(max_range + 1, false);
		for (const auto& field : fields) {
			for (const auto& range : field.second) {
				for (int i = range.min; i < range.max + 1; i++) {
					allowable_values[i] = true;
				}
			}
		}
	}

	bool is_number_valid(int num) const {
		if (num < allowable_values.size() && allowable_values[num] == true)
			return true;
		return false;
	}
};


InputContents InputContents::parse_input(const char* filename) {
	std::ifstream file(filename);
	InputContents input;
	std::string buf;
	int max_range = 0;

	// The input is broken up into three blocks, so we'll look for the edges of them.
	while (std::getline(file, buf)) {
		if (buf == "")
			break;
		auto split_buf = split(buf, ": ");
		auto ranges_s = split(split_buf[1], " or ");
		//input.fields[split_buf[0]] = range;
		std::vector<Range> ranges;
		for (const auto& range_s : ranges_s) {
			auto bounds_s = split(range_s, "-");
			std::vector<int> bounds;
			for (auto bound_s : bounds_s) {
				int bound = std::stoi(bound_s);
				bounds.push_back(bound);
				if (bound > max_range) max_range = bound;
			}
			ranges.emplace_back(Range(bounds[0], bounds[1]));
		}
		input.fields[split_buf[0]] = ranges;
	}

	// Remaining blocks
	auto v_to_i = [&](const std::vector<std::string>& vec, std::vector<int>& dest) {
		for (const auto& num_s : vec) {
			dest.emplace_back(std::stoi(num_s));
		}
	};
	// My ticket
	std::getline(file, buf);  // discard block label
	while (std::getline(file, buf)) {
		if (buf == "")
			break;
		auto numbers_s = split(buf, ",");
		v_to_i(numbers_s, input.my_ticket);
	}

	// Nearby tickets
	std::getline(file, buf);  // discard block label
	while (std::getline(file, buf)) {
		if (buf == "")
			break;
		auto numbers_s = split(buf, ",");
		input.nearby_tickets.emplace_back(std::vector<int>());
		v_to_i(numbers_s, input.nearby_tickets[input.nearby_tickets.size() - 1]);
	}
	input.get_allowable_values(max_range);
	return input;
}

int main() {
	Timer timer;
	auto input = InputContents::parse_input("input.txt");
	int result = input.scanning_error_rate();
	std::cout << "Scanning error rate: " << result << "\n";
	long long result2 = input.departure_fields_product();
	std::cout << "Departure fields product: " << result2 << "\n";
}