import re


class Rule:
    def __init__(self, line):
        line = line.strip().split(" contain ")
        line[1] = line[1].strip(".").split(", ")
        self.contents = {}
        for item in line[1]:
            # Grab that number out in front
            regex = re.compile(r"[0-9]+")
            # If we didn't find one that means it's no bags inside
            if match := regex.match(item):
                quantity = int(item[match.span()[0]:match.span()[1]])
                # The +1 deals with the space
                bag_type = item[match.span()[1] + 1:]
                if quantity > 1:
                    # This gets rid of the s if it's plural
                    bag_type = bag_type[:-1]

                self.contents[bag_type] = quantity
        # The s makes things irritating so I want it gone
        self.bag_type = line[0][:-1]

    def contains_directly(self, bag_type: str):
        if bag_type in self.contents:
            return True
        else:
            return False

    # Warning: recursive
    def contains(self, bag_type: str, rule_dict: dict):
        if self.contains_directly(bag_type):
            return True
        else:
            for bag in self.contents:
                if bag in rule_dict:
                    if rule_dict[bag].contains(bag_type, rule_dict):
                        return True
                else:
                    print("An unexpected bag was discovered!")
            return False

    def count_internal_bags(self, rule_dict: dict):
        internal_bags = 0
        for bag in self.contents:
            # count these bags...
            internal_bags += self.contents[bag]  # recall that this value represents the quantity
            # ...and count the bags inside of it
            internal_bags += rule_dict[bag].count_internal_bags(rule_dict) * self.contents[bag]
        return internal_bags


def parse_input(filename: str):
    with open(filename, "r") as file:
        rules = []
        for line in file:
            rules.append(Rule(line))
        return rules


def main():
    rules = parse_input("input.txt")
    rule_dict = {}
    for rule in rules:
        print(f"{rule.bag_type} contains {rule.contents}")
        rule_dict[rule.bag_type] = rule
    shiny_gold = 0
    for rule_entry in rule_dict.keys():
        rule = rule_dict[rule_entry]
        if rule.contains("shiny gold bag", rule_dict):
            print(f"Found {rule.contents} in {rule.bag_type}")
            shiny_gold += 1

    print(f"Found {shiny_gold} bags containing at least one shiny gold bag.")

    print(f"A shiny gold bag contains {rule_dict['shiny gold bag'].count_internal_bags(rule_dict)} bags")


if __name__ == "__main__":
    main()
