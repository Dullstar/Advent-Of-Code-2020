import itertools as it
import math


class Adapter:
    # Pycharm is annoying and spellchecks these, and I don't feel like trying to figure out how to turn it off
    # right now - hence, voltage
    def __init__(self,  voltage_rating: int):
        self.voltage_rating = voltage_rating


def parse_input(filename):
    output = []
    with open(filename, "r") as file:
        for line in file:
            output.append(Adapter(int(line.strip())))
    return output


# Assumes the adapters list is sorted.
def get_deltas(adapters: list, output: bool = True):
    prev_voltage = 0
    delta_dict = {}
    for adapter in adapters:
        delta = adapter.voltage_rating - prev_voltage
        # I hope this works if there wasn't a value there before; guessing it won't though
        if delta not in delta_dict:
            delta_dict[delta] = 1
        else:
            delta_dict[delta] += 1
        prev_voltage = adapter.voltage_rating

    if output:
        # The following may not be safe for arbitrary inputs, since we've assumed some stuff exists
        # Accounts for a provision in the problem regarding the final adapter in the chain
        delta_dict[3] += 1
        print("Distribution:")
        for key in sorted(delta_dict.keys()):
            print(f"Delta = {key} occurs {delta_dict[key]} times.")
        print(f"Product of 1 and 3 differences: {delta_dict[1] * delta_dict[3]}")
    return delta_dict


'''def find_valid_connections(adapters: list):
    # Challenge: find ALL valid arrangements such that the maximum difference is at most 3.
    # Fortunately, the sorted list should help...

    total_valid = 0
    final_voltage = adapters[-1].voltage_rating + 3
    # Let's cut down the number of combinations to test by figuring out the smallest one that'll work
    smallest_possible_combo = math.ceil(adapters[-1].voltage_rating/3)
    for i in range(smallest_possible_combo, len(adapters) + 1):
        for adapter_combo in it.combinations(adapters, i):
            adapter_combo = list(adapter_combo)
            # The last one needs to be in the chain
            adapter_combo.append(Adapter(final_voltage))
            maximum_delta = max(get_deltas(adapter_combo, output=False).keys())
            print("Size: ", len(adapter_combo), end="; ")
            print("Voltages: ", end="")
            for adapter in adapter_combo:
                print(adapter.voltage_rating, end=" ")
            if maximum_delta <= 3:
                # print("Size: ", len(adapter_combo), end="; ")
                # print("Voltages: ", end="")
                # for adapter in adapter_combo:
                #     print(adapter.voltage_rating, end=" ")
                print("\t\tPasses: maximum delta: ", maximum_delta)
                total_valid += 1
            else:
                print("\t\tFails: maximum delta: ", maximum_delta)
    print(f"A total of {total_valid} valid arrangements were found.")
    return'''


def find_next(start: int, adapters_ints: list):
    # Recursion termination conditions: hit the end, or run out of further leads
    found = 0
    if start == adapters_ints[-1]:
        print("Terminate check: found a valid result.")
        return 1
    for i in range(1, 4):
        if start + i in adapters_ints:
            found += find_next(start + i, adapters_ints)
    print("Terminate check: hit dead end from start = ", start)
    return found


def find_valid_connections(adapters_non_int: list):
    adapters = [adapter.voltage_rating for adapter in adapters_non_int]
    found = find_next(0, adapters)
    print(f"Found {found} possibilities")
    return


def main():
    adapters = parse_input("test_input.txt")
    adapters2 = parse_input("test_input2.txt")
    adapters3 = parse_input("input.txt")

    adapters.sort(key=lambda adapter: adapter.voltage_rating)
    adapters2.sort(key=lambda adapter: adapter.voltage_rating)
    adapters3.sort(key=lambda adapter: adapter.voltage_rating)

    get_deltas(adapters)
    print()
    get_deltas(adapters2)
    print()
    get_deltas(adapters3)

    find_valid_connections(adapters)
    find_valid_connections(adapters2)
    find_valid_connections(adapters3)


if __name__ == "__main__":
    main()
