module Day_24;

import std.stdio;
import std.string;
import std.algorithm;
import core.time;

struct point3d {
    int x;
    int y;
    int z;
    this(int _x, int _y, int _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    // I'm not really sure anything other than add/subtract would make sense, but
    // I think there's more to learn from trying to write it this way - in a more
    // serious project we can always restrict the overloads to only the ones that
    // actually make sense.
    point3d opBinary(string op)(in point3d other) {
        mixin ("return point3d(x" ~ op ~ "other.x, y" ~ op ~ "other.y, z" ~ op ~ "other.z);");
    }

    void opOpAssign(string op)(in point3d other) {
        mixin("x " ~ op ~ "= other.x;");
        mixin("y " ~ op ~ "= other.y;");
        mixin("z " ~ op ~ "= other.z;");
    }

    unittest {
        writeln("Unit test: point3d");
        point3d a = point3d(0, 1, 2);
        point3d b = point3d(1, 3, 5);
        point3d c = a + b;
        point3d d = a - b;
        point3d e = a * b;
        point3d f = point3d(0, 0, 0);
        f += a;
        assert (c == point3d(1, 4, 7));
        writeln("    Expected: 1, 4, 7; result: ", c.x, ", ", c.y, ", ", c.z);
        assert (d == point3d(-1, -2, -3));
        writeln("    Expected: -1, -2, -3; result: ", d.x, ", ", d.y, ", ", d.z);
        assert (e == point3d(0, 3, 10));
        writeln("    Expected: 0, 3, 10; result: ", e.x, ", ", e.y, ", ", e.z);
        assert (a == f);
        writeln("    Expected: 0, 1, 2; result: ", f.x, ", ", f.y, ", ", f.z);
    }
}

class Tile {
public:
    point3d location;
    bool active;
    this (point3d _location, bool _active) {
        location = _location;
        active = _active;
        m_neighbors = [direction.se + location, 
                       direction.sw + location,
                       direction.ne + location,
                       direction.nw + location,
                       direction.e + location,
                       direction.w + location];
    }
    void push_active(Floor layout) {
        if (active) {
            
            foreach (const ref neighbor; m_neighbors) {
                // Tile* other_tile = layout.get_tile_ptr(point3d(neighbor.x + location.x, neighbor.y + location.y, neighbor.z + location.z));
                Tile* other_tile = layout.get_tile_ptr(point3d(neighbor.x, neighbor.y, neighbor.z));
                other_tile.m_active_neighbors++;
            }
        }
    }

    bool update_active() {
        // writeln("Tile ", location.x, ", ", location.y, ", ", location.z, ": ", m_active_neighbors, " active neighbors");
        if ((active && (m_active_neighbors == 1 || m_active_neighbors == 2))
            || (!active && m_active_neighbors == 2))
            active = true;
        else active = false;
        m_active_neighbors = 0;
        return active;
    }

private:
    const point3d[6] m_neighbors;
    int m_active_neighbors = 0;
}

enum direction : point3d {
    e = point3d(1, -1, 0),
    se = point3d(0, -1, 1),
    sw = point3d(-1, 0, 1),
    w = point3d(-1, 1, 0),
    nw = point3d(0, 1, -1),
    ne = point3d(1, 0, -1)
}

class Floor {
public:
    this() {
        const ref_tile = point3d(0, 0, 0);
    }

    point3d find_tile(direction[] instruction) const {
        point3d coord = point3d(ref_tile.x, ref_tile.y, ref_tile.z);
        foreach (dir; instruction) {
            coord += dir;
        }
        return coord;
    }
    unittest {
        writeln("Unit test: Tile directions");
        Floor floor = new Floor;
        auto directions = [direction.nw, direction.w, direction.sw, direction.e, direction.e];
        point3d result = floor.find_tile(directions);
        writeln("    Expected: 0, 0, 0; result: ", result.x, ", ", result.y, ", ", result.z);
        assert(result == point3d(0, 0, 0));
        auto directions2 = [direction.e, direction.se, direction.w];
        result = floor.find_tile(directions2);
        writeln("    Expected: 0, -1, 1; result: ", result.x, ", ", result.y, ", ", result.z);
        assert(result == point3d(0, -1, 1));
        auto directions3 = [direction.w, direction.ne];
        result = floor.find_tile(directions3);
        writeln("    Expected: 0, 1, -1; result: ", result.x, ", ", result.y, ", ", result.z);
        assert(floor.find_tile(directions3) == point3d(0, 1, -1));
    }

    void flip_tile(point3d coords) {
        Tile* ptr = coords in m_tiles;
        // writeln("Flipping tile: ", coords.x, ", ", coords.y, ", ", coords.z);
        if (ptr is null)
            m_tiles[coords] = new Tile(coords, true);
        else
            ptr.active = !(ptr.active);
    }
    unittest {
        writeln("Unit test: Tile flipping");
        Floor floor = new Floor;
        auto tile1 = point3d(0, 0, 0);
        auto tile2 = point3d(0, -1, 1);
        floor.flip_tile(tile1);
        writeln("    Expected: tile1 is flipped (i.e. set to true): Result: ", floor.m_tiles[tile1].active);
        assert(floor.m_tiles[tile1].active);
        floor.flip_tile(tile2);
        writeln("    Expected: tile2 is flipped (i.e. set to true): Result: ", floor.m_tiles[tile2].active);
        assert(floor.m_tiles[tile2].active);
        floor.flip_tile(tile1);
        writeln("    Expected: tile1 is not flipped (i.e. set to false): Result: ", floor.m_tiles[tile1].active);
        assert(!floor.m_tiles[tile1].active);
    }

    int count_tiles() {
        // if there's a way to use count from std.algorithm on an associative array, I'm not sure what it is
        int total = 0;
        int revert = 0;
        foreach (tile; m_tiles) {
            if (tile.active) total++;
            else revert++;
        }
        return total;
    }

    int tile_cellular_automata_update() {
        foreach (tile; m_tiles) {
            tile.push_active(this);
        }
        // Need to make sure every tile is push_active'd before proceeding;
        // otherwise active_neighbor counts won't be correct.
        int total = 0;
        foreach (tile; m_tiles) {
            if (tile.update_active) total++;
        }
        return total;
    }

    Tile* get_tile_ptr(const point3d point) {
        Tile* ptr = (point in m_tiles);
        if (ptr is null) {
            m_tiles[point] = new Tile(point, false);
            ptr = &m_tiles[point];
        }
        return ptr;
    }

private:
    Tile[point3d] m_tiles;
    point3d ref_tile;
}

auto parse_input(string filename) {
    File file = File(filename);
    char[] buffer;
    string raw_contents;
    while (file.readln(buffer)) {
        raw_contents ~= buffer;
    }
    string[] instruction_strings = raw_contents.strip.split("\n");
    direction[][] instructions;
    foreach (instruction_str; instruction_strings) {
        instruction_str = strip(instruction_str);
        size_t start = 0;
        direction[] instruction;
        while (instruction_str[start..$].length > 0) {
            if (instruction_str[start] == 'e') {
                instruction ~= direction.e;
                start++;
            }
            else if (instruction_str[start] == 'w') {
                instruction ~= direction.w;
                start ++;
            }
            else if (instruction_str[start] == 's') {
                if (instruction_str[start + 1] == 'w')
                    instruction ~= direction.sw;
                else if (instruction_str[start + 1] == 'e')
                    instruction ~= direction.se;
                start += 2;
            }
            else if (instruction_str[start] == 'n') {
                if (instruction_str[start + 1] == 'w')
                    instruction ~= direction.nw;
                else if (instruction_str[start + 1] == 'e')
                    instruction ~= direction.ne;
                start += 2;
            }
            else writeln("This shouldn't be reachable.");
        }
        instructions ~= instruction;
    }
    return instructions;
}

void main() {
    auto start_time = MonoTime.currTime;
    direction[][] instructions = parse_input("input.txt");
    auto floor = new Floor;
    foreach(instruction; instructions) {
        floor.flip_tile(floor.find_tile(instruction));
    }
    writeln("Flipped tiles: ", floor.count_tiles);
    auto end_time1 = MonoTime.currTime;
    for (int i = 1; i < 100; i++) {
        floor.tile_cellular_automata_update;
    }
    writeln("Day 100", ": ", floor.tile_cellular_automata_update, " tiles");
    auto end_time2 = MonoTime.currTime;
    writeln("Elapsed time (pt 1): ", float((end_time1 - start_time).total!"usecs") / 1000, " ms");
    writeln("Elapsed time (pt 2): ", float((end_time2 - end_time1).total!"usecs") / 1000, " ms");
    writeln("Elapsed time (total): ", float((end_time2 - start_time).total!"usecs") / 1000, " ms");
}
