def parse_input(filename: str):
    with open(filename, "r") as file:
        lines = []
        for line in file:
            lines.append(line.strip())
        return lines


def get_offset(letter: str, total_seats: int):
    offset = -1
    if letter == "F" or letter == "L":
        offset = 0
    elif letter == "B" or letter == "R":
        offset = total_seats // 2

    return offset


def interpret_pass(boarding_pass: str):
    current_num = 128
    offsets = []
    for i in range(7):
        offsets.append(get_offset(boarding_pass[i], current_num))
        current_num //= 2
    row = sum(offsets)
    offsets = []
    current_num = 8
    for i in range(7, 10):
        offsets.append(get_offset(boarding_pass[i], current_num))
        current_num //=2
    column = sum(offsets)
    return row, column


def main():
    passes = parse_input("input.txt")
    seats = []
    for boarding_pass in passes:
        row, column = interpret_pass(boarding_pass)
        seats.append(row * 8 + column)
    max_seat_ID = max(seats)
    min_seat_ID = min(seats)
    print(f"The highest seat ID is {max_seat_ID}")
    print(f"The lowest seat ID is {min_seat_ID}")

    ids_to_check = [x for x in range(min_seat_ID + 1, max_seat_ID)]  # don't need to check max and min
    for seat_id in ids_to_check:
        if seat_id not in seats:
            print(f"The seat ID {seat_id} is not in the list of seats")
            break  # we've been told there should only be one, so we can stop when we find it


if __name__ == "__main__":
    main()