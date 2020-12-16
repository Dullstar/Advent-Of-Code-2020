#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

#include "stringsplit.h"
#include "timer.h"

class MemGame {
public:
	std::vector<int> memory;  // too big for std::array
	int turn_count;
	int previous_num;
	MemGame(std::vector<int> numbers) 
		: memory(30000000, -1), previous_num(0)
	{
		turn_count = 1;
		for (const auto& number : numbers) {
			memory[number] = turn_count;
			previous_num = number;
			turn_count++;
		}
		memory[previous_num] = -1;
	}

	void process_turn() {
		int next;
		int previous_turn = turn_count - 1;
		if (memory[previous_num] == -1) {  // wasn't found
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