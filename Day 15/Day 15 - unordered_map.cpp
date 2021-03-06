// Note: this one should use less memory than the other one, but it's slower.
// Still a useful demonstration of the speed difference between std::vector and std::unordered_map
// for a situation where they can both work

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

#include "stringsplit.h"
#include "timer.h"

class MemGame {
public:
	std::unordered_map<int, int> memory;
	int turn_count;
	int previous_num;
	MemGame(std::vector<int> numbers) {
		turn_count = 1;
		for (const auto& number : numbers) {
			memory[number] = turn_count;
			previous_num = number;
			turn_count++;
		}
		memory.erase(previous_num);
	}

	void process_turn() {
		int next;
		int previous_turn = turn_count - 1;
		if (memory.find(previous_num) == memory.end()) {  // wasn't found
			next = 0;
		}
		else next = previous_turn - memory[previous_num];

		memory[previous_num] = previous_turn;
		turn_count++;
		previous_num = next;
	}
	
	// Note that we stop at this turn without doing it, so if you want to run through turn 20, you need to specify 21.
	void process_to(int stop) {
		while (turn_count < stop)
			process_turn();
		std::cout << "Turn " << turn_count - 1 << ": " << previous_num << "\n";
	}
};

std::vector<int> parse_input(const char* filename) {
	std::vector<int> output;
	std::ifstream file(filename);
	std::stringstream raw_contents;
	raw_contents << file.rdbuf();
	auto raw_entries = split(strip(raw_contents.str()), ",");
	for (const auto& entry : raw_entries) {
		output.push_back(std::stoi(entry));
	}
	return output;
}

int main() {
	Timer timer;
	auto numbers = parse_input("input.txt");
	MemGame game(numbers);
	game.process_to(2021);
	game.process_to(30000001);
}