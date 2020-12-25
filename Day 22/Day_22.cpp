#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>
#include <cctype>
#include <unordered_set>
#include <unordered_map>

#include "stringsplit.h"
#include "timer.h"

struct Deck {
public:
	std::queue<int32_t> cards;
	Deck(std::string raw_deck) {
		auto str_raw_deck = split(strip(raw_deck), "\n");
		for (const auto& entry : str_raw_deck) {
			if (entry[0] == 'P') continue;  // discard the section header
			cards.push(std::stoi(entry));
		}
	}

	Deck() = default;

	Deck(std::queue<int32_t> source_cards, int32_t num_cards) {  // intentional pass by value
		while (num_cards > 0) {
			num_cards--;
			cards.push(source_cards.front());
			source_cards.pop();
		}
	}

	int64_t score() {
		int64_t current_card_multiplier = static_cast<int64_t>(cards.size());
		int64_t score = 0;
		while (!cards.empty()) {
			int64_t current_card_value = static_cast<int64_t>(cards.front());
			cards.pop();
			score += current_card_multiplier * current_card_value;
			current_card_multiplier--;
		}
		return score;
	}

	// For what we're using this for, it would be better to write a hash function,
	// but this is easier.
	std::string to_string() {
		auto cards_copy = cards;
		std::string output;
		while (!cards_copy.empty()) {
			output += std::to_string(cards_copy.front()) + ',';
			cards_copy.pop();
		}
		return strip(output, ",");
	}
};

class Game {
public:
	bool finished;
	int32_t winning_player;
	Game(const char* filename)
		: finished(false), winning_player(-1) {
		std::ifstream file(filename);
		std::stringstream sstream;
		sstream << file.rdbuf();
		auto decks = split(sstream.str(), "\n\n");
		player1 = Deck(decks[0]);
		player2 = Deck(decks[1]);
	}

	Game(const std::queue<int32_t>& player1_deck, const std::queue<int32_t>& player2_deck,
		int32_t player1_card, int32_t player2_card)
		: finished(false), winning_player(-1), 
		player1(player1_deck, player1_card), player2(player2_deck, player2_card) {}

	virtual void process_round() {
		auto player1_card = player1.cards.front();
		auto player2_card = player2.cards.front();
		player1.cards.pop();
		player2.cards.pop();

		auto give_cards_to_player = [&](Deck& player) {
			player.cards.push(std::max(player1_card, player2_card));
			player.cards.push(std::min(player1_card, player2_card));
		};

		if (player1_card > player2_card) give_cards_to_player(player1);
		else give_cards_to_player(player2);

		auto declare_winner = [&](int32_t player_num) {
			finished = true;
			winning_player = player_num;
			std::cout << "The winner is: Player " << player_num << "\n";
		};
		if (player1.cards.empty())
			declare_winner(2);
		else if (player2.cards.empty())
			declare_winner(1);
	}

	int64_t winning_score() {
		if (winning_player == 1) return player1.score();
		else if (winning_player == 2) return player2.score();
		return -1;
	}

protected:
	Deck player1;
	Deck player2;
};

class RecursiveGame : public Game {
public:
	RecursiveGame(const char* filename)
		: Game(filename), original(true) {
		id = next_id;
		next_id++;
	};
	RecursiveGame(const std::queue<int32_t>& player1_deck, const std::queue<int32_t>& player2_deck, 
		int32_t player1_card, int32_t player2_card)
		: Game(player1_deck, player2_deck, player1_card, player2_card), original(false) {
		id = next_id;
		next_id++;
	}

	void process_round() override {
		rounds++;
		bool repeated = identify_repeat_decks();
		auto player1_card = player1.cards.front();
		auto player2_card = player2.cards.front();
		player1.cards.pop();
		player2.cards.pop();
		auto declare_winner = [&](int32_t player_num) {
			finished = true;
			winning_player = player_num;
			recursive_game_results[to_string()] = player_num;
			if (original) std::cout << "The winner is: Player " << player_num << "\n";
		};
		if (!repeated) {
			find_and_handle_round_winner(player1_card, player2_card);

			if (player1.cards.empty())
				declare_winner(2);
			else if (player2.cards.empty())
				declare_winner(1);
		}
		else
			declare_winner(1);
	}

	std::string to_string() {
		return "P1:" + player1.to_string() + ";P2:" + player2.to_string();
	}
private:
	static std::unordered_map<std::string, int32_t> recursive_game_results;
	bool original;
	std::unordered_set<std::string> past_decks_p1;
	std::unordered_set<std::string> past_decks_p2;
	static int32_t next_id;
	int32_t id = -1;
	int32_t rounds = 1;

	bool identify_repeat_decks() {
		auto p1_deck = player1.to_string();
		auto p2_deck = player2.to_string();
		auto p1_past = past_decks_p1.find(p1_deck);
		if (p1_past != past_decks_p1.end()) return true;
		auto p2_past = past_decks_p2.find(p2_deck);
		if (p2_past != past_decks_p2.end()) return true;
		past_decks_p1.insert(p1_deck);
		past_decks_p2.insert(p2_deck);
		return false;
	}

	void find_and_handle_round_winner(int32_t player1_card, int32_t player2_card) {
		auto give_cards_to_player = [&](Deck& player, int32_t winning_card, int32_t losing_card) {
			player.cards.push(winning_card);
			player.cards.push(losing_card);
		};
		if (static_cast<int32_t>(player1.cards.size()) >= player1_card
			&& static_cast<int32_t>(player2.cards.size()) >= player2_card) {
			auto rec_game_winner = run_recursive_game(player1_card, player2_card);
			if (rec_game_winner == 1) {
				give_cards_to_player(player1, player1_card, player2_card);
			}
			else 
				give_cards_to_player(player2, player2_card, player1_card);
		}
		else {
			if (player1_card > player2_card)
				give_cards_to_player(player1, player1_card, player2_card);
			else
				give_cards_to_player(player2, player2_card, player1_card);
		}
	}

	int32_t run_recursive_game(int32_t player1_card, int32_t player2_card) {
		// ideally we wouldn't be constructing this every time, but this works to handle the slicing
		RecursiveGame rec_game(player1.cards, player2.cards, player1_card, player2_card);
		auto str = rec_game.to_string();
		// doesn't help much with speed, but doesn't really hurt either, so I haven't reverted this.
		// it was worth a try
		auto search = recursive_game_results.find(str);
		if (search != recursive_game_results.end()) return recursive_game_results[str];
		while (!rec_game.finished) {
			rec_game.process_round();
		}
		return rec_game.winning_player;
	}
};

std::unordered_map<std::string, int32_t> RecursiveGame::recursive_game_results;
int32_t RecursiveGame::next_id = 1;

int main() {
	const char* filename = "input.txt";
	Timer overall_timer;
	{
		Timer part1_timer;
		Game game(filename);
		while (!game.finished) {
			game.process_round();
		}
		std::cout << "The winning score was: " << game.winning_score() << "\n";
		std::cout << "Part 1: "; // intentionally omitted newline; will mix with timer output
	}
	{
		Timer part2_timer;
		RecursiveGame rec_game(filename);
		while (!rec_game.finished) {
			rec_game.process_round();
		}
		std::cout << "The winning score for the recursive game was: " << rec_game.winning_score() << "\n";
		std::cout << "Part 2: ";
	}
	std::cout << "Overall time: ";
}