from Day_20_Tiles import *
import time
import queue as q


class Image:
    def __init__(self, tilepool: list[Tile]):
        self.tilepool = tilepool
        tilepool[0].location = (0, 0)
        self.layout = {(0, 0): tilepool[0]}
        self.tile_size = self.tilepool[0].width
        self.full_layout = ""
        self.part_1_answer = 0
        queue = q.SimpleQueue()
        queue.put(tilepool[0])

        while not queue.empty():
            tile: Tile = queue.get()
            for other_tile in tilepool:
                if other_tile.location is None and tile.place_if_border_matches(other_tile):
                    self.layout[other_tile.location] = other_tile  # it won't be None anymore here
                    queue.put(other_tile)

        self.finalize()
        self.full_layout = Tile("Tile 0000:\n" + self.full_layout.strip())

    def __str__(self):
        # for key in sorted(self.layout.keys(), key=self._tile_location_compare):
        return self.layout.__str__()

    def finalize(self):
        coords = {}
        x_set = set()
        for key in self.layout.keys():
            x_set.add(key[0])
            if key[1] in coords:
                coords[key[1]].append(key)
            else:
                coords[key[1]] = [key]

        for y in sorted(coords.keys()):
            coords[y].sort()
            for j in range(1, self.tile_size - 1):
                for coord in coords[y]:
                    for i in range(1, self.tile_size - 1):
                        self.full_layout += "#" if self.layout[coord][i, j] == 1 else "."
                self.full_layout += "\n"

        # print(self.full_layout)

        x_min = min(x_set)
        x_max = max(x_set)
        y_min = min(coords.keys())
        y_max = max(coords.keys())
        self.part_1_answer = int(self.layout[x_min, y_min].tile_id) * int(self.layout[x_max, y_min].tile_id) \
            * int(self.layout[x_min, y_max].tile_id) * int(self.layout[x_max, y_max].tile_id)

        print("Product of corners: ", self.part_1_answer)


def parse_input(filename: str):
    with open(filename, "r") as file:
        tile_strings = file.read().strip().split("\n\n")
        tiles = []
        for tile_string in tile_strings:
            tiles.append(Tile(tile_string))

    return tiles


def main():
    start_time = time.time()
    filename = "input.txt"
    image = Image(parse_input(filename))
    end_time_pt_1 = time.time()
    image.full_layout.search_for_sea_monsters()
    end_time_pt_2 = time.time()
    print("Elapsed time (overall): ", end_time_pt_2 - start_time)
    print("Elapsed time (part 1): ", end_time_pt_1 - start_time)
    print("Elapsed time (part 2): ", end_time_pt_2 - end_time_pt_1)


if __name__ == "__main__":
    main()
