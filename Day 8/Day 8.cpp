#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "stringsplit.h"

enum class opcode {
	acc,
	jmp,
	nop
};

struct Instruction {
	opcode opcode;
	int arg;
};

class Console {
public:
	Console() : m_accumulator(0), m_program_counter(0) {}
	int execute_command(const Instruction& instruction) {
		switch (instruction.opcode) {
		case opcode::acc:
			acc(instruction.arg);
			break;
		case opcode::jmp:
			jmp(instruction.arg);
			break;
		case opcode::nop:
			nop();
			break;
		}
		return m_program_counter;
	}
	int accumulator() { return m_accumulator; }
	int program_counter() { return m_program_counter; }
private:
	int m_accumulator;
	int m_program_counter;
	void acc(int arg) {
		m_accumulator += arg;
		++m_program_counter;
	}
	void jmp(int arg) {
		m_program_counter += arg;
	}
	void nop() {
		++m_program_counter;
	}
};

std::vector<Instruction> parse_input(const std::string& filename) {
	std::vector<Instruction> code;
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line)) {
		auto raw_instruction = split(strip(line), " ");
		int arg;
		opcode op;
		if (raw_instruction[0] == "acc") op = opcode::acc;
		else if (raw_instruction[0] == "jmp") op = opcode::jmp;
		else op = opcode::nop;
		std::stringstream sstream(raw_instruction[1]);
		sstream >> arg;
		code.emplace_back(Instruction{ op, arg });
	}
	return code;
}

void report(bool succeeded, int accumulator, int program_counter) {
	if (succeeded) {
		printf("The accumulator was at %d when the program completed at program counter %d\n\n", accumulator, program_counter);
	}
	else {
		printf("The accumulator status was %d at program counter %d\n", accumulator, program_counter);
		puts("at the moment an instruction first ran for the second time\n");
	}
}

struct result {
	bool completed;
	int accumulator;
	int program_counter;
};

std::string opcode_to_string(opcode op) {
	if (op == opcode::acc) return "acc";
	else if (op == opcode::jmp) return "jmp";
	else return "nop";
}

void report_change(opcode orig_opcode, int orig_arg, opcode new_opcode, int new_arg) {
	std::cout << "Original: " << opcode_to_string(orig_opcode) << ", " << orig_arg << "\n"
		<< "New:      " << opcode_to_string(new_opcode) << ", " << new_arg << "\n";
}

result run_code(const std::vector<Instruction>& code, bool suppress_fail = false) {
	Console console;
	std::vector<int> visited;
	int next_command = 0;
	bool completed = false;
	while (!completed) {
		if (next_command >= static_cast<int>(code.size())) completed = true;
		// if next_command in visited
		else if (std::any_of(visited.cbegin(), visited.cend(), [&](int a) { return a == next_command; })) break;
		else {
			visited.push_back(next_command);
			next_command = console.execute_command(code[next_command]);
		}
	}

	if (completed || !suppress_fail)
		report(completed, console.accumulator(), console.program_counter());
	return result { completed, console.accumulator(), console.program_counter() };
}

result correct_code(std::vector<Instruction> code) {  // pass by value intentional; I do in fact want to copy
	int attempts = 0;
	auto try_swap_opcode = [&](Instruction& cmd, opcode find, opcode replace) {
		result output { false, -1, -1 };
		++attempts;
		if (cmd.opcode == find) {
			cmd.opcode = replace;
			output = run_code(code, true);
			if (output.completed) {
				report_change(find, cmd.arg, replace, cmd.arg);
			}
			else {
				cmd.opcode = find;  // put it back how we found it, since we're only allowed to change one.
			}
		}
		return output;
	};
	for (auto& instruction : code) {
		result final_output{ false, -1, -1 };
		if (instruction.opcode == opcode::jmp)
			final_output = try_swap_opcode(instruction, opcode::jmp, opcode::nop);
		else if (instruction.opcode == opcode::nop) {
			final_output = try_swap_opcode(instruction, opcode::nop, opcode::jmp);
		}
		if (final_output.completed) {
			std::cout << "Attempts: " << attempts << "\n";
			return final_output;
		}
	}
	// If we haven't returned yet, nothing was found.
	std::cout << "Failed to find possible fix.";
	return result { false, -1, -1 };
}

int main() {
	auto code = parse_input("input.txt");
	run_code(code);
	correct_code(code);
}