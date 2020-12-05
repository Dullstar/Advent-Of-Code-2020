def parse_input(filename: str):
    with open(filename, "r") as file:
        lines = file.read()
        list_of_dictionaries = []
        # Split 1: Separates different entries
        lines = lines.strip().split("\n\n")
        for line in lines:
            # Split 2: Separates different fields within a single entry
            line = line.split()
            dictionary = {}
            for item in line:
                # Split 3: Separates fields from their values
                item = item.split(":")
                dictionary[item[0]] = item[1]
            list_of_dictionaries.append(dictionary)
        return list_of_dictionaries


def validate_entry_count(entry: dict):
    required_keys = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
    print("Checking entry: ", entry)
    for key in required_keys:
        if key not in entry:
            print("This entry is missing key: ", key)
            print()
            return False
    print("No (important) missing keys detected.")
    return True


# This function is separate so we can still print out the results from Puzzle 1
# without having to completely overhaul the return values from the validate_entry_count()
#
# There's definitely more elegant ways to do this, but let's not over-engineer this script -
# it isn't really production code, after all!
def validate_entry_values(entry: dict):
    required_keys = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
    for key in required_keys:
        if key == "byr":
            byr = int(entry[key])
            if byr < 1920 or byr > 2002:
                print(f"Birth year of {byr} is not between 1920 and 2002")
                return False
        elif key == "iyr":
            iyr = int(entry[key])
            if iyr < 2010 or iyr > 2020:
                print(f"Issue year of {iyr} is not between 2010 and 2020")
                return False
        elif key == "eyr":
            eyr = int(entry[key])
            if eyr < 2020 or eyr > 2030:
                print(f"Expiration year of {eyr} is not between 2020 and 2030")
                return False
        elif key == "hgt":
            value = ""
            unit = ""
            stop_checking_numbers = False
            for i in range(len(entry[key])):
                # Probably over-engineered considering the input file
                char = entry[key][i]
                if not stop_checking_numbers and char.isnumeric():
                    value += char
                    continue
                elif not stop_checking_numbers:
                    stop_checking_numbers = True
                unit += char
            value = int(value)

            if unit == "in":
                if value < 59 or value > 76:
                    print("The airline discriminates against people who are too tall or too short,")
                    print(f"and a height of {value} inches is not between 59 and 76")
                    return False
            elif unit == "cm":
                if value < 150 or value > 193:
                    print("The airline discriminates against people who are too tall or too short,")
                    print(f"and a height of {value} cm is not between 150 and 193")
                    return False
            else:
                print(f"Unit {unit} is not either in or cm")
                return False
        elif key == "hcl":
            hcl_valid = True
            if entry[key][0] != "#" or len(entry[key]) != 7:
                hcl_valid = False
            else:
                try:
                    # If this value cannot be converted to hex, i.e. does not consist of 0-9 and a-f,
                    # then this throws a ValueError, telling us this isn't valid
                    int(entry[key][1:], 16)
                except ValueError:
                    hcl_valid = False
            if not hcl_valid:
                print(f"Hair color {entry[key]} does not comply to expected format #nnnnnn where n is a hex number")
                return False
        elif key == "ecl":
            allowed_values = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]
            if entry[key] not in allowed_values:
                print(f"Eye color {entry[key]} not in allowed list of colors: {allowed_values}")
                return False
        elif key == "pid":
            if len(entry[key]) != 9:
                print(f"The Passport ID {entry[key]} does not consist of 9 digits.")
                return False
            elif not entry[key].isnumeric():
                print(f"The Passport ID {entry[key]} is not a numeric value.")
                return False
    # If you've made it here, congratulations! It's a valid result.
    print("All entries are valid.")
    return True


def main():
    entries = parse_input("input.txt")
    valid_entries = 0
    valid_entries_2 = 0
    total_entries = 0
    for entry in entries:
        if validate_entry_count(entry):
            valid_entries += 1
            # This way we don't even try to validate the values if some of them are missing,
            # since that means there's already a problem with it.
            if validate_entry_values(entry):
                valid_entries_2 += 1
            print()
        total_entries += 1

    print(f"{valid_entries} out of {total_entries} are valid (Puzzle 1).")
    print(f"{valid_entries_2} out of {total_entries} are valid (Puzzle 2).")


if __name__ == '__main__':
    main()
