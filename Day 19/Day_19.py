import re
import time


def parse_input(filename: str, part_num: int = 1):
    with open(filename) as file:
        contents = file.read().strip().split("\n\n")
    rules_ls = contents[0].split("\n")
    known = {}
    rules = {}
    for i, rule in enumerate(rules_ls):
        rule = rule.split(": ")
        rules[rule[0]] = rule[1]

    regex = re.compile(r"[a-zA-Z]")
    for key, rule in rules.items():
        match = regex.search(rule)
        if match:
            known[key] = match.group(0)
            rules[key] = match.group(0)

    if part_num == 2:
        rules["8"] = "42 | 42 8"
        rules["11"] = "42 31 | 42 11 31"
        for i in range(3):
            # Credit to KBD2 on the OneLoneCoder discord for this loop; the thing I was trying to do wouldn't work;
            # they literally wrote these four lines for me, and that made the rest of the code start working, so I'd
            # say that definitely deserves some credit!
            rules["8"] = rules["8"].replace("8", "(" + rules["8"] + ")")
            rules["11"] = rules["11"].replace("11", "(" + rules["11"] + ")")
        rules["8"] = rules["8"].replace("8", "")
        rules["11"] = rules["11"].replace("11", "")

    find_nums = re.compile(r"[0-9]")
    while len(known) < len(rules):
        new_known = {}
        for rule_regex, rule_string in known.items():
            rule_regex = re.compile(fr"\b{rule_regex}\b")
            for key, rule in rules.items():
                # Update the rule with new information
                rule = rule_regex.sub(f"(?:{rule_string})", rule)
                rules[key] = rule
                # Check if this rule is finished yet
                match = find_nums.search(rule)
                if match is None:
                    rule = re.sub(r" ", "", rule)
                    new_known[key] = rule
                    rules[key] = rule
        known = new_known

    return rules, contents[1].split("\n")


def main():
    start_time = time.time()
    rules, messages = parse_input("input.txt", 1)

    # print(rules["0"])
    total = 0
    rule_regex = re.compile(rules["0"])
    for message in messages:
        match = rule_regex.match(message)
        if match and len(match.group(0)) == len(message):
            total += 1
    print(total)

    rules, messages = parse_input("input.txt", 2)
    # print(rules["0"])
    total = 0
    rule_regex = re.compile(rules["0"])
    for message in messages:
        match = rule_regex.match(message)
        if match and len(match.group(0)) == len(message):
            total += 1

    print(total)

    end_time = time.time()
    print("Elapsed time: ", end_time - start_time, "s")


if __name__ == "__main__":
    main()
