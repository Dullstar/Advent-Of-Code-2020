import time


class Seat:
    def __init__(self, x: int, y: int):
        self.occupied = False
        self.occupied_neighbors = 0
        self.neighbors = []
        self.x = x
        self.y = y


class Layout:
    def __init__(self, seats: dict, max_x: int, max_y: int):
        self.seats = seats
        self.max_x = max_x
        self.max_y = max_y

    def reset_layout(self):
        for seat in self.seats.values():
            seat.occupied = False
            seat.occupied_neighbors = 0
            seat.neighbors = []

    def init_seat_neighbors_p1(self):
        potential_neighbors = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
        for seat in self.seats.values():
            for relative_coord in potential_neighbors:
                neighbor_coords = relative_coord[0] + seat.x, relative_coord[1] + seat.y
                if neighbor_coords in self.seats:
                    seat.neighbors.append(self.seats[neighbor_coords])

    def init_seat_neighbors_p2(self):
        search_directions = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
        for seat in self.seats.values():
            for direction in search_directions:
                current = direction[0] + seat.x, direction[1] + seat.y
                while (current[0] >= 0) and (current[0] < self.max_x) \
                        and (current[1] >= 0) and (current[1] < self.max_y):
                    if current in self.seats:
                        seat.neighbors.append(self.seats[current])
                        break
                    else:
                        current = direction[0] + current[0], direction[1] + current[1]

    def occupy_seats(self, occupancy_tolerance):
        changes = True
        while changes:
            changes = False
            # Push presence to neighbors
            for seat in self.seats.values():
                if seat.occupied:
                    for neighbor_seat in seat.neighbors:
                        neighbor_seat.occupied_neighbors += 1

            # Update seats
            for seat in self.seats.values():
                if seat.occupied and (seat.occupied_neighbors > occupancy_tolerance):
                    seat.occupied = False
                    changes = True
                elif (not seat.occupied) and (seat.occupied_neighbors == 0):
                    seat.occupied = True
                    changes = True
                seat.occupied_neighbors = 0  # reset this for the next round

    def count_occupied_seats(self) -> int:
        occupied_seats = 0
        for seat in self.seats.values():
            occupied_seats += int(seat.occupied)
        return occupied_seats


def parse_input(filename: str) -> Layout:
    with open(filename, "r") as file:
        contents = file.read().strip().split("\n")
        seats = {}
        line_count = 0
        for line in contents:
            char_count = 0
            for char in line:
                if char == ".":
                    pass
                elif char == "L":
                    seats[char_count, line_count] = Seat(char_count, line_count)
                else:
                    raise ValueError(f"Unexpected character in input: {char}")
                char_count += 1
            line_count += 1
        return Layout(seats, char_count, line_count)


def main():
    start_time = time.time()
    layout = parse_input("input.txt")
    layout.init_seat_neighbors_p1()
    layout.occupy_seats(3)
    print("Occupied seats (part 1): ", layout.count_occupied_seats())
    end_time_1 = time.time()
    print(f"Elapsed time (part 1): {(end_time_1 - start_time) * 1000:.2f} ms\n")
    layout.reset_layout()
    layout.init_seat_neighbors_p2()
    layout.occupy_seats(4)
    print("Occupied seats (part 2): ", layout.count_occupied_seats())
    end_time_2 = time.time()
    print(f"Elapsed time (part 2): {(end_time_2 - end_time_1) * 1000:.2f} ms\n")
    print(f"Elapsed time (overall): {(end_time_2 - start_time) * 1000:.2f} ms")

if __name__ == "__main__":
    main()
