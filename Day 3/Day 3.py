class Map:
    def __init__(self, filename):
        self.map = []
        with open(filename, "r") as file:
            for line in file:
                self.map.append(line.strip())
        self.len_x = len(self.map[0])
        self.len_y = len(self.map)

    def get_pos(self, x, y):
        if y >= self.len_y:  # Signifies we've reached the bottom
            return None
        else:
            return self.map[y][x % self.len_x]

    def count_trees(self, dx, dy):
        trees = 0
        x = 0
        y = 0
        current_pos = None
        while True:
            current_pos = self.get_pos(x, y)
            if current_pos is None:
                break
            elif current_pos == "#":
                trees += 1
            x += dx
            y += dy
        return trees
        

# For organizational purposes
class SlopeResult:
    def __init__(self, dx, dy, trees):
        self.dx = dx
        self.dy = dy
        self.trees = trees


def main():
    # For convenience, today we'll be switching to hard coding the inputs, since getting command line arguments is trivial
    # and just takes time.

    tree_map = Map("input.txt")
    #trees = tree_map.count_trees(3, 1)
    #print(f"{trees} trees were encountered.")

    results = [SlopeResult(1, 1, 0), SlopeResult(3, 1, 0), SlopeResult(5, 1, 0), SlopeResult(7, 1, 0), SlopeResult(1, 2, 0)]
    multiplication_result = 1
    for result in results:
        result.trees = tree_map.count_trees(result.dx, result.dy)
        print(f"{result.trees} trees were encountered on slope Right {result.dx}, Down {result.dy}")
        multiplication_result *= result.trees

    # This commented out line was a careless error, but this wasn't the behavior I expect
    # from this line:
    # shouldn't result.trees be out of scope and therefore unprintable?
    # print(f"The product of treees encountered is {result.trees}.")
    print(f"The product of trees encountered is {multiplication_result}.")


main()
