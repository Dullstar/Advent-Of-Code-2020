#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Map {
public:
	Map(const char* filename) {
		std::ifstream file;
		file.open(filename);
		std::string line;
		while (std::getline(file, line)) {
			m_layout.emplace_back(line);
		}
	}

	int count_trees(int dx, int dy) {
		int trees = 0;
		int x = 0;
		int y = 0;

		auto get_pos = [&](int x, int y) {
			if (y >= (int)m_layout.size()) return -1;
			char c = m_layout[y][x % m_layout[y].size()];
			if (c == '#') return 1;
			else return 0;
		};

		while (true) {
			int item_at_pos = get_pos(x, y);
			if (item_at_pos == -1) break;
			else if (item_at_pos == 1) trees++;
			x += dx;
			y += dy;
		}

		return trees;
	}
private:
	std::vector<std::string> m_layout;
};

// For organizational purposes
struct SlopeResult {
	int dx;
	int dy;
	int trees;
	SlopeResult(int _dx, int _dy, int _trees) : dx(_dx), dy(_dy), trees(_trees) {}
};

int main() {
	// For convenience, today we'll be switching to hard coding the inputs, since getting command line arguments is trivial
	// and just takes time

	Map tree_map("input.txt");
	std::vector<SlopeResult> results;
	results.emplace_back(1, 1, 0);
	results.emplace_back(3, 1, 0);
	results.emplace_back(5, 1, 0);
	results.emplace_back(7, 1, 0);
	results.emplace_back(1, 2, 0);
	long long multiplication_result = 1;

	for (auto& result : results) {
		result.trees = tree_map.count_trees(result.dx, result.dy);
		std::cout << result.trees << " trees were encountered on slope Right " << result.dx << ", Down " << result.dy << "\n";
		multiplication_result *= result.trees;
	}

	std::cout << "The product of trees encountered is " << multiplication_result << "\n";
}