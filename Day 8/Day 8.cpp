#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "stringsplit.h"

enum class opcode {
	acc,
	jmp,
	nop
};

std::string opcode_to_string(opcode op) {
	if (op == opcode::acc) return "acc";
	else if (op == opcode::jmp) return "jmp";
	else return "nop";
}

struct Instruction {
	opcode opcode;
	int arg;
};

class Console {
public:
	Console() : m_accumulator(0), m_program_counter(0) {}
	int accumulator() { return m_accumulator; }
	int program_counter() { return m_program_counter; }
	void execute_command(const Instruction& instruction) {
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
	}
	bool run_all(const std::vector<Instruction>& code) {
		reset();
		std::vector<bool> visited(code.size(), false);
		while (m_program_counter < code.size()) {
			if (visited[m_program_counter]) return false;
			visited[m_program_counter] = true;
			execute_command(code[m_program_counter]);
		}
		return true;
	}
	
	void fix_code(std::vector<Instruction> code) {  // intentional passing by value for safety reasons
		for (auto& cmd : code) {
			opcode orig = cmd.opcode;
			switch (cmd.opcode) {
			case opcode::jmp:
				cmd.opcode = opcode::nop;
				break;
			case opcode::nop:
				cmd.opcode = opcode::jmp;
				break;
			default:
				continue;
			}
			if (run_all(code) == true) {
				std::cout << "Original: " << opcode_to_string(orig) << " " << cmd.arg
					<< "\tReplacement: " << opcode_to_string(cmd.opcode) << " " << cmd.arg
					<< "    Final program counter: " << m_program_counter << "\n";
			}
			cmd.opcode = orig;
		}
	}
	void reset() {
		m_accumulator = 0;
		m_program_counter = 0;
	}
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

int main() {
	auto code = parse_input("input.txt");
	Console console;
	console.run_all(code);
	std::cout << "Stopped at program counter " << console.program_counter() << " with accumulator at " << console.accumulator() << "\n";
	console.fix_code(code);
	std::cout << "Accumulator was at " << console.accumulator() << " when the program terminated.\n";
}