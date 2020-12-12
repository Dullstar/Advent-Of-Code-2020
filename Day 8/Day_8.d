module Day_8;

import std.stdio;
import std.algorithm;
import std.range;
import std.conv;

// Basically just trying this language out here,
// so don't expect perfection or anything...

enum opcode {
    acc,
    jmp,
    nop
}

struct Instruction {
public:
    opcode cmd;
    int arg;
    this(opcode cmd_, int arg_) {
        cmd = cmd_;
        arg = arg_;        
	}
}

class Console {
public:
    @property int accumulator() { return m_accumulator; }
    @property int program_counter() { return m_program_counter; }

    void execute_command(ref Instruction cmd) {
        final switch (cmd.cmd) {
        case opcode.acc:
            acc(cmd.arg);
            break;
        case opcode.jmp:
            jmp(cmd.arg);
            break;
        case opcode.nop:
            nop();
            break;
		}
	}

    void fix_code(ref Instruction[] code) {
		Instruction[] code_copy = code[];
        foreach (ref cmd; code_copy) {
            opcode orig = cmd.cmd;
            switch (cmd.cmd) {
            case opcode.jmp:
                cmd.cmd = opcode.nop;
                break;
            case opcode.nop:
                cmd.cmd = opcode.jmp;
                break;
            default:
                continue;
			}
            if (run_all(code_copy) == true) {
                writeln("Original: ", orig, " ", cmd.arg, "\tReplacement: ", cmd.cmd, " ", cmd.arg);
                writeln("    Final program counter: ", program_counter);
                break;
			}
            cmd.cmd = orig;
		}
	}

    bool run_all(ref Instruction[] code) {
        reset();
		// an associateive array would be more memory efficient,
        // but this should be faster.
        bool[] visited;  // syntax note: bools should initialize to false by default
        visited.length = code.length;
		foreach (cmd; code) {
            if (program_counter >= code.length) break;
            if (visited[program_counter]) return false;
			visited[program_counter] = true;
            execute_command(code[program_counter]);
		}
        return true;
	}

private:
    void acc(int arg) {
        m_accumulator += arg;
        m_program_counter++;
	}
    void jmp(int arg) {
        m_program_counter += arg;
	}
    void nop() {
        m_program_counter++;
	}
    void reset() {
        m_program_counter = 0;
        m_accumulator = 0;
	}
    int m_program_counter = 0;
    int m_accumulator = 0;
}

auto parse_input(string filename) {
    string output;
    char[] buffer;
    auto file = File(filename);
    while (file.readln(buffer)) {
        // from the example I suppose this probably appends buffer to output,
        // but looking into that bit of syntax more is probably wise long-term
        output ~= buffer;
	}
    string[] lines = output.split("\n");
    Instruction[] commands;
	foreach (cmd; lines) {
        if (cmd != "") {
            string[2] contents = cmd.split(" ");
            opcode op;
            final switch (contents[0])
			{
            case "acc":
                op = op.acc;
                break;
            case "jmp":
                op = op.jmp;
                break;
            case "nop":
                op = op.nop;
                break;
			}
            // There's probably a better way; seems like adding stuff to a dynamic array
            // doesn't work quite like using a std::vector in C++
			Instruction[1] command = Instruction(op, to!int(contents[1]));
            commands ~= command;
		}
	}
    return commands;
}

void main() {
    Instruction[] code = parse_input("input.txt");
    Console console = new Console;
    console.run_all(code);
    writeln("Stopped at program counter ", console.program_counter, " with accumulator at ", console.accumulator);
    console.fix_code(code);
    writeln("Accumulator was at ", console.accumulator, " when the program terminated.\n");
    writeln("Press any key...");
    readln();
}
