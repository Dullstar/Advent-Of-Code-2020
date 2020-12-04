import sys

class Pass:
    def __init__(self, pass1, pass2):
        self.pass1 = pass1
        self.pass2 = pass2
def main():
    if len(sys.argv) < 2:
        print ("Required command line argument: input filename")
        return

    try:
        with open(sys.argv[1], "r") as file:
            passing1 = 0
            passing2 = 0
            for line in file:
                pass_status = check_line(line)
                if pass_status.pass1 == True:
                    passing1 += 1
                if pass_status.pass2 == True:
                    passing2 += 1
        print(f"Puzzle 1: {passing1} passwords passed")
        print(f"Puzzle 2: {passing2} passwords passed")

    except OSError:
        print ("Failed to open file: ", sys.argv[1])


def check_line(line):
    # Format: {int}-{int} {char}: {string}
    line = line.strip().split()
    numbers = line[0]
    letter = line[1][0]
    password = line[2]

    numbers = numbers.split("-")
    min = int(numbers[0])
    max = int(numbers[1])
    if min > max:
        temp = min
        min = max
        max = temp

    # Puzzle 1
    num_occurrences = password.count(letter)
    if num_occurrences <= max and num_occurrences >= min:
        pass1 = True
    else:
        pass1 = False

    # Puzzle 2
    pos1 = min - 1  # Same input source as puzzle 1, but puzzle 2 interprets them differently.
    pos2 = max - 1  # 1 is subtracted because an input of 1 refers to the first character, at index 0
        
    if (password[pos1] == letter) != (password[pos2] == letter):
        pass2 = True
    else:
        pass2 = False

    return Pass(pass1, pass2)

main()