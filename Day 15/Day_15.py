import time


class MemGame:
    def __init__(self, numbers: list):
        self.memory = {}
        self.turn_count = 1
        for number in numbers:
            self.memory[number] = self.turn_count
            self.previous_num = number
            self.turn_count += 1
        del self.memory[self.previous_num]  # Don't want to remember the last value quite yet
        print(self.memory)

    def process_turn(self):
        previous_turn = self.turn_count - 1
        if self.previous_num in self.memory:
            _next = previous_turn - self.memory[self.previous_num]
        else:
            _next = 0

        self.memory[self.previous_num] = previous_turn
        self.turn_count += 1
        self.previous_num = _next

    def process_to(self, stop: int):
        """Note: the stop condition works the same way slices do, so to go up to and including 2020, set stop to 2021"""
        while self.turn_count < stop:
            self.process_turn()
        print(f"Turn {self.turn_count - 1}: {self.previous_num}")


def parse_input(filename):
    with open(filename, "r") as file:
        return [int(i) for i in file.read().strip().split(",")]


def main():
    start_time = time.time()
    numbers = parse_input("input.txt")
    game = MemGame(numbers)
    game.process_to(2021)
    game.process_to(30000001)
    print(game.previous_num)
    print("Elapsed time:", time.time() - start_time)


if __name__ == "__main__":
    main()