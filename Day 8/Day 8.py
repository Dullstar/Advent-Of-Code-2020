class Instruction:
    def __init__(self, opcode: str, arg: int or str):
        self.arg = int(arg)
        self.opcode = opcode


class Console:
    def __init__(self):
        self.accumulator = 0
        self.program_counter = 0

    def acc(self, arg: int):
        self.accumulator += arg
        self.program_counter += 1

    def jmp(self, arg: int):
        self.program_counter += arg

    def nop(self):
        self.program_counter += 1

    def execute_command(self, instruction: Instruction):
        if instruction.opcode == "acc":
            self.acc(instruction.arg)
        elif instruction.opcode == "jmp":
            self.jmp(instruction.arg)
        elif instruction.opcode == "nop":
            self.nop()
        return self.program_counter


def parse_input(filename: str):
    code = []
    with open(filename, "r") as file:
        for line in file:
            line = line.strip().split()
            code.append(Instruction(line[0], line[1]))
    return code


def report(succeeded: bool, accumulator: int, program_counter: int):
    if succeeded:
        print(f"The accumulator was at {accumulator} when the program completed at program counter {program_counter}")
    else:
        print(f"The accumulator status was {accumulator} at program counter {program_counter}")
        print("at the moment an instruction first ran for the second time")
    print()


# This function isn't actually important to solve the problem, but I thought it would be a nice touch
# to be able to see what exactly is different between the original and the new one.
def report_change(original: Instruction, new: Instruction):
    print(f"Original: {original.opcode}, {original.arg}")
    print(f"New:      {new.opcode}, {new.arg}")


def run_code(code: list, suppress_fail: bool = False):
    visited = []
    console = Console()
    next_command = 0
    completed = False
    # Part 1
    while not completed:
        if next_command >= len(code):
            completed = True
        elif next_command in visited:
            break
        else:
            visited.append(next_command)
            next_command = console.execute_command(code[next_command])

    if completed or not suppress_fail:
        report(completed, console.accumulator, console.program_counter)
    return completed, console.accumulator, console.program_counter


def correct_code(code: list):
    new_code = code.copy()
    output = None
    attempts = 0  # Output flavor
    for cmd in new_code:
        if cmd.opcode == "jmp":
            cmd.opcode = "nop"
            output = run_code(new_code, suppress_fail=True)
            if output[0]:
                report_change(Instruction("jmp", cmd.arg), cmd)
                print("Attempts: ", attempts + 1)
                return output
            else:
                cmd.opcode = "jmp"  # put it back how we found it, since we're only allowed to change one.
                attempts += 1
        elif cmd.opcode == "nop":
            cmd.opcode = "jmp"
            output = run_code(new_code, suppress_fail=True)
            if output[0]:
                report_change(Instruction("nop", cmd.arg), cmd)
                print("Attempts: ", attempts + 1)
                return output
            else:
                attempts += 1
                cmd.opcode = "nop"  # put it back how we found it, since we're only allowed to change one.

    # If we haven't returned yet, nothing was found.
    print("Failed to find possible fix.")
    return False, None, None


def main():
    code = parse_input("input.txt")
    visited = []
    console = Console()
    next_command = 0
    # Part 1
    output = run_code(code)

    correct_code(code)


if __name__ == "__main__":
    main()
