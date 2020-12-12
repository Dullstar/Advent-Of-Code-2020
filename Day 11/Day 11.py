class Seat:
    def __init__(self, exists: bool, occupied: bool):
        # Giving the seat an exists flag instead of taking advantage of the ability to mix None and objects in a list
        # is mostly just to help the IDE out.
        self.exists = exists
        self.occupied = occupied


class Layout:
    def __init__(self, layout: str):
        layout = layout.split("\n")
        # handle end of file newline if present
        if len(layout[-1]) == 0:
            del layout[-1]
        self.x = len(layout[0])
        self.y = len(layout)
        self.layout = []

        for line in layout:
            for char in line:
                if char == "L":
                    self.layout.append(Seat(True, False))
                else:
                    self.layout.append(Seat(False, False))
        # We can use this to reset things if we need to.
        self.original_layout = [Seat(seat.exists, seat.occupied) for seat in self.layout]

    def get_seat_index(self, x: int, y: int, warn=True):
        index = (y * self.x) + x
        if x < 0 or y < 0 or x >= self.x or y >= self.y:
            if warn:
                print(f"Tried accessing invalid coordinate ({x}, {y}); maximum is ({self.x - 1}, {self.y - 1})")
                print(self.layout[-1])
            raise IndexError
        return index

    # This probably repeats a lot of work, but we'll wait to think of optimizations until it becomes a problem
    def get_num_occupied_adjacent_seats(self, x: int, y: int):
        # Relative coordinates of seats to check.
        seats_to_check = [(0, 1), (-1, -1), (-1, 1), (1, 1), (1, -1), (-1, 0), (1, 0), (0, -1)]
        total = 0
        for seat in seats_to_check:
            seat = (x + seat[0], y + seat[1])
            # Bounds check
            if (seat[0] < 0) or (seat[0] >= self.x) or (seat[1] < 0) or (seat[1] >= self.y):
                continue
            index = self.get_seat_index(seat[0], seat[1])
            if self.layout[index].exists and self.layout[index].occupied:
                total += 1
        return total

    def get_num_occupied_visible_seats(self, x: int, y: int):
        # Relative coordinates of directions to check.
        directions_to_check = [(0, 1), (-1, -1), (-1, 1), (1, 1), (1, -1), (-1, 0), (1, 0), (0, -1)]
        total = 0
        for direction in directions_to_check:
            if self.get_if_occupied_seats_in_direction(x, y, direction):
                total += 1
        return total

    # Note: start on the source seat. This will look ahead.
    def get_if_occupied_seats_in_direction(self, x, y, direction: tuple):
        try:
            index = self.get_seat_index(x + direction[0], y + direction[1], warn=False)
        except IndexError:
            return False
        if self.layout[index].exists:
            if self.layout[index].occupied:
                return True
            else:
                return False
        else:
            # We want to propagate this directional check until we hit a chair or the edge.
            return self.get_if_occupied_seats_in_direction(x + direction[0], y + direction[1], direction)

    # It's a bit hacky, but it's a more DRY way of implementing Part 2 than just copying this function.
    # Part 1 gets the default values, Part 2's changes have to be explicitly passed. Backwards compatibility ;)
    def occupy_seats(self, rounds=1, max_seats=4, scenario=1):
        # We don't want the iteration ordering to affect the results, so we make our changes to a copy first.
        # And apparently we have to use a list comprehension to do this, because this is basically a list of
        # pointers because of how Python works. Extremely irritating.
        new_layout = [Seat(seat.exists, seat.occupied) for seat in self.layout]
        for y in range(self.y):
            for x in range(self.x):
                index = self.get_seat_index(x, y)
                # self.debug_adjacent_seats(x, y)
                if not self.layout[index].exists:
                    continue
                if scenario == 1:
                    nearby_seats = self.get_num_occupied_adjacent_seats(x, y)
                else:
                    nearby_seats = self.get_num_occupied_visible_seats(x, y)

                if (not self.layout[index].occupied) and nearby_seats == 0:
                    new_layout[index].occupied = True
                elif nearby_seats >= max_seats:
                    new_layout[index].occupied = False

        found = True
        for i in range(len(new_layout)):
            if (new_layout[i].occupied != self.layout[i].occupied or
                    new_layout[i].exists != self.layout[i].exists):
                found = False
                break
        if found:
            return
        self.layout = new_layout
        # self.print()  # Kinda slow, but useful for taking a peak inside the magic box.
        self.occupy_seats(rounds + 1, max_seats, scenario)

    def count_seats(self):
        total = 0
        for seat in self.layout:
            if seat.occupied:
                total += 1
        return total

    def print(self):
        for y in range(self.y):
            for x in range(self.x):
                check = self.get_seat_index(x, y)
                if not self.layout[check].exists:
                    print(".", end="")
                elif self.layout[check].occupied:
                    print("#", end="")
                else:
                    print("L", end="")
            print()

    def reset_simulation(self):
        self.layout = [Seat(seat.exists, seat.occupied) for seat in self.original_layout]


def parse_input(filename):
    with open(filename, "r") as file:
        layout = Layout(file.read())
    return layout


def main():
    # Parsing
    layout = parse_input("input.txt")
    # Part 1
    layout.occupy_seats()
    print("Scenario 1 Equilibrium: ", layout.count_seats())
    # Part 2
    layout.reset_simulation()
    layout.occupy_seats(max_seats=5, scenario=2)
    print("Scenario 2 Equilibrium: ", layout.count_seats())


if __name__ == "__main__":
    main()
