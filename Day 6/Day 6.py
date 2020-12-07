def parse_input(filename):
    with open(filename, "r") as file:
        raw_contents = file.read()
        groups = raw_contents.strip().split("\n\n")

    return groups


def get_unique_answers(group: str):
    questions = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
                 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']
    q_dict = {}
    for question in questions:
        q_dict[question] = False

    for char in group:
        # extra stuff like newlines may still be present, but it's harmless
        if char in q_dict:
            q_dict[char] = True

    output = 0
    for key in q_dict.keys():
        if q_dict[key]:
            output += 1

    return output


def get_shared_answers(group: str):
    questions = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
                 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']
    q_dict = {}
    for question in questions:
        q_dict[question] = 0

    individuals = group.split("\n")
    for individual in individuals:
        for answer in individual:
            # I don't think this check is necessary given the input, but extra safety won't hurt,
            # there's probably not enough input to make this performance critical
            if answer in q_dict:
                q_dict[answer] += 1

    output = 0
    for key in q_dict.keys():
        if q_dict[key] == len(individuals):
            output += 1

    return output


def main():
    groups = parse_input("input.txt")
    unique_answers = 0
    shared_answers = 0

    for group in groups:
        unique_answers += get_unique_answers(group)
        shared_answers += get_shared_answers(group)

    print(f"Unique answers per group: {unique_answers}")
    print(f"Shared answers per group: {shared_answers}")


if __name__ == "__main__":
    main()
