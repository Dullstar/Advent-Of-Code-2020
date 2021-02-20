#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "stringsplit.h"
#include "timer.h"


class CupGame {
public:
	std::vector<int32_t> cups;
	CupGame(std::vector<int32_t> cups_raw) 
	: cups(cups_raw.size(), -1), current_cup(cups_raw[0]){	
		for (size_t i = 0; i < cups_raw.size(); i++) {
			//size_t next_i = (i + 1) == cups_raw.size() ? i + 1 : 0;
			size_t next_i = i + 1;
			if (next_i == cups_raw.size()) next_i = 0;
			cups[cups_raw[i]] = cups_raw[next_i];
		}
	}

	void move_cups() {
		auto next1 = next_index(current_cup);
		auto next2 = next_index(next1);
		auto next3 = next_index(next2);

		// pull these cups out
		cup_storage[0] = next1;
		cup_storage[1] = next2;
		cup_storage[2] = next3;
		// severs linkage beteeen starting cup and first removed cup and replaces it with the last removed cups ex-neighbor
		cups[current_cup] = cups[next3];

		// Find destination cup
		int32_t dest_cup = current_cup;
		do {
			dest_cup--;
			if (dest_cup == -1) dest_cup = static_cast<int32_t>(cups.size()) - 1;
		} while (dest_cup == cup_storage[0] || dest_cup == cup_storage[1] || dest_cup == cup_storage[2]);

		// re-establish linkage with the removed cups in their new home
		size_t new_neighbor = cups[dest_cup];
		cups[dest_cup] = cup_storage[0];
		cups[cup_storage[2]] = new_neighbor;

		// update current cup
		current_cup = next_index(current_cup);
	}

	void print_order() {
		auto start = cups[0];
		auto next = start;
		do {
			std::cout << next + 1;
			next = cups[next];
		} while (next != start);
		std::cout << "\n";
	}

	// Account for the very specific format it wants instead of just printing the raw order
	void print_output_pt1() {
		auto start = cups[0];
		auto next = start;
		std::cout << "Part 1 solution: ";
		do {
			std::cout << next + 1;
			next = cups[next];
		} while (next != 0);
		std::cout << "\n";
	}

	void print_output_pt2() {
		size_t n1 = next_index(0);
		size_t n2 = next_index(n1);
		int64_t product = (static_cast<int64_t>(n1) + 1) * (static_cast<int64_t>(n2) + 1);
		// std::cout << n1 + 1 << "\n";
		// std::cout << n2 + 1 << "\n";
		std::cout << "Part 2 solution: " << product << "\n";
	}

private:
	int32_t current_cup;
	std::array<int32_t, 3> cup_storage{0, 0, 0};
	size_t next_index(size_t cup) {
		return cups[cup];
	}

};

CupGame parse_input(const char* filename, size_t cups_max = 9) {
	std::ifstream file(filename);
	std::stringstream sstream;
	sstream << file.rdbuf();
	std::string cup_seqeunce = strip(sstream.str());
	std::vector<int32_t> cups_raw;
	for (const char c : cup_seqeunce) {
		cups_raw.push_back(std::stoi(&c) - 1);
	}

	while (cups_raw.size() < cups_max) {
		cups_raw.push_back(static_cast<int32_t>(cups_raw.size()));
	}
	return CupGame(cups_raw);
}

int main() {
	Timer timer_overall;
	const char* filename = "input.txt";
	{
		Timer timer_pt1;
		CupGame cups = parse_input(filename);
		for (int i = 0; i < 100; i++) {
			cups.move_cups();
		}
		cups.print_output_pt1();
		std::cout << "Part 1 ";
	}
	{
		Timer timer_pt2;
		CupGame cups2 = parse_input(filename, 1000000);
		for (int i = 0; i < 10000000; i++) {
			cups2.move_cups();
		}
		cups2.print_output_pt2();
		std::cout << "Part 2 ";
	}
	std::cout << "Overall ";
}