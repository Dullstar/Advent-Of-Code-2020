#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <iostream>

std::vector<std::string> parse_input(const std::string& filename) {
	std::ifstream file(filename);
	std::string line;
	std::vector<std::string> output;
	while (std::getline(file, line)) {
		output.emplace_back(line);
	}
	return output;
}

int get_offset(char letter, int total_seats) {
	int offset = -1;
	if (letter == 'F' || letter == 'L')
		offset = 0;
	else if (letter == 'B' || letter == 'R')
		offset = total_seats / 2;

	return offset;
}

std::pair<int, int> interpret_pass(const std::string& boarding_pass) {
	int current_num = 128;
	std::vector<int> offsets;
	for (int i = 0; i < 7; i++) {
		offsets.push_back(get_offset(boarding_pass[i], current_num));
		current_num /= 2;
	}
	int row = std::accumulate(offsets.begin(), offsets.end(), 0);
	current_num = 8;
	offsets.clear();
	for (int i = 7; i < 10; i++) {
		offsets.push_back(get_offset(boarding_pass[i], current_num));
		current_num /= 2;
	}
	int column = std::accumulate(offsets.begin(), offsets.end(), 0);
	return std::pair<int, int>(row, column);
}

int main() {
	auto passes = parse_input("input.txt");
	std::vector<int> seats;
	for (const auto& boarding_pass : passes) {
		// 1st = row, 2nd = column
		std::pair<int, int> seat_pos = interpret_pass(boarding_pass);
		seats.push_back(seat_pos.first * 8 + seat_pos.second);
	}
	std::sort(seats.begin(), seats.end());
	// Because we just sorted, we know where these are; no need to search for them.
	int max_seat_ID = *(seats.end() - 1);
	int min_seat_ID = *seats.begin();
	std::cout << "The highest seat ID is " << max_seat_ID << "\n";
	std::cout << "The lowest seat ID is " << min_seat_ID << "\n";

	// Assumes seats is sorted (which it is)
	for (int i = 1; i < static_cast<int>(seats.size()); i++) {
		if (seats[i] != seats[i - 1] + 1) {
			std::cout << "Our seat ID is " << seats[i - 1] + 1 << "\n";
			break;
		}
	}

}