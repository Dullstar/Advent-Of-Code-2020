import enum


class Operator(enum.Enum):
    ADD = enum.auto()
    MUL = enum.auto()


# Note that this problem involves changing the order of operations.
# A lot of these are static methods packaged with this class for organization purposes - having them keep the object's
# state intact allows us to more safely reuse bits and pieces in Part 2.

# Flaw: this assumes that each term is in the range 0 - 9. It could be fixed with some slightly more sophisticated
# parsing, but since we know it's not the case it'll run faster to assume. Ideally we'd add a parsing step to fix
# this.
class Expression:
    def __init__(self, string: str):
        self.terms = []
        check = True
        resume_check = -1
        string = self.remove_spaces(string)
        for i, char in enumerate(string):
            if check:
                if char == "(":
                    check = False
                    expr, resume_check = self.find_subexpression(string, i)
                    self.terms.append(expr)
                else:
                    self.terms.append(char)
            elif i >= resume_check:
                check = True
        self._result = None
        self._result_2 = None

    @property
    def result(self):
        """Gets the result of the expression."""
        if self._result is not None:
            return self._result
        else:
            self._result = self.evaluate(self.terms, 1)
            return self._result

    @property
    def result_2(self):
        """Gets the result of the expression for Part 2, where addition is to be done first"""
        if self._result_2 is not None:
            return self._result_2
        else:
            self._result_2 = self.evaluate_2(self.terms)
            return self._result_2

    @staticmethod
    def find_subexpression(string: str, offset: int):
        """Creates a subexpression from parentheses given the location of the starting parenthesis.
        Returns the expression and the index of the final parenthesis"""
        to_ignore = 0
        offset += 1  # We don't need to check the original character that triggered this call
        for i, char in enumerate(string[offset:], offset):
            if char == "(":
                to_ignore += 1
            if char == ")":
                if to_ignore > 0:
                    to_ignore -= 1
                    continue
                else:
                    return Expression(string[offset:i]), i
        print(string[offset:])
        assert 0, "Triggered find_subexpression, but didn't find an end. Either the code is wrong or a parenthesis " \
                  "in the original expression is mismatched."

    @staticmethod
    def remove_spaces(string):
        """Removes spaces from the given expression. It returns the modified expression, not modify it in place."""
        output = ""
        for char in string:
            if char != " ":
                output += char
        return output

    @staticmethod
    def evaluate(terms: list, rule_set: int):
        """Evaluates the expression given by an already-parsed list of terms. Can be explicitly called, but it's better
        to call this through the result property, which will make the call if it needs to."""
        running_total = 0
        current_operand = Operator.ADD  # defaulting to add will make it convenient to get started
        for term in terms:
            if term == "+":
                current_operand = Operator.ADD
                continue
            if term == "*":
                current_operand = Operator.MUL
                continue

            if type(term) == Expression:
                if rule_set == 1:
                    term = term.result
                elif rule_set == 2:
                    term = term.result_2
                else:
                    assert 0, "Unrecognized rule set ID: {rule_set}"

            term = int(term)

            if current_operand is Operator.ADD:
                running_total += term
            elif current_operand is Operator.MUL:
                running_total *= term
            else:
                assert 0, "No operator currently loaded. This should never happen, and if it does, it's a bug."
        return running_total

    @staticmethod
    def evaluate_2(terms: list):
        """Evaluates the expression by the second rule_set. Like the evaluate function, it can be explicitly called, 
        but it's still better to call this through the result_2 property, which will make the call if it needs to."""

        add_terms = []
        mul_terms = []
        for term in terms:
            if term == "*":
                mul_terms.append(Expression.evaluate(add_terms, 2))
                add_terms = []
            else:
                add_terms.append(term)
        mul_terms.append(Expression.evaluate(add_terms, 2))
        product = 1
        for term in mul_terms:
            if type(term) == Expression:
                term = term.result_2
            product *= term

        self._result_2 = product
        return product


def parse_input(filename):
    with open(filename, "r") as file:
        expressions = [Expression(expression) for expression in [line.strip() for line in file]]
    results1 = [expression.result for expression in expressions]
    results2 = [expression.result_2 for expression in expressions]
    return results1, results2


def main():
    results1, results2 = parse_input("input.txt")
    print(f"All the expressions left to right sum to {sum(results1)}")
    print(f"All the expressions addition before multiplication sum to {sum(results2)}")


if __name__ == "__main__":
    main()
