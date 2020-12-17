module Day_17_pt2;

import std.stdio;
import std.string;
import core.time;
import std.conv;

// A largely copy/paste version of Part 1, but now it's 4d.

// The alternative was having to work a bunch ifs in.

// I'm not sure I like either option, really.

struct point_4d {
    int x;
    int y;
    int z;
    int w;
    this(int _x, int _y, int _z, int _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
}

class Hypercube {
public:
    this(int x, int y, int z, int w, bool active) {
        this(point_4d(x, y, z, w), active);
    }
    this(point_4d point, bool active) {
        m_point = point;
        m_active = active;
    }

    @property bool active() {
        return m_active;
    }

    @property point_4d point() {
        return m_point;
    }

    void apply_changes() {
        m_active = m_will_be_active;
    }

    bool check_neighbors(HypercubeContainer cubes) {
        int number_neighbors = m_neighbors.length.to!int;
        int active_neighbors = 0;
        foreach(neighbor; m_neighbors) {
            if (neighbor.active) {
                active_neighbors++;
            }
        }

        if (m_active) {
            if (active_neighbors == 2 || active_neighbors == 3) {
                m_will_be_active = true;
            }
            else m_will_be_active = false;
        }
        else if (!m_active && active_neighbors == 3)
            m_will_be_active = true;
        else m_will_be_active = false;

        /*if (m_will_be_active) {
            foreach(neighbor; m_neighbors) {
                if (neighbor.m_neighbors.length != 80) {
                    cubes.give_cube_neighbors(*neighbor);
                }
            }
        }*/

        return m_will_be_active;
    }

    void link_to_neighbors(ref Hypercube[point_4d] cubes) {
        foreach (x; m_point.x - 1 .. m_point.x + 2)
            foreach (y; m_point.y - 1 .. m_point.y + 2)
                foreach(z; m_point.z - 1 .. m_point.z + 2)
                    foreach(w; m_point.w - 1 .. m_point.w + 2) {
                        if (x == m_point.x && y == m_point.y && z == m_point.z && w == m_point.w) continue;
                        // this time, we DON'T want to create new neighbors.
                        Hypercube* ptr = point_4d(x, y, z, w) in cubes;
                        if (ptr !is null) m_neighbors ~= ptr;
                    }
    }

private:
    Hypercube*[] m_neighbors;
    point_4d m_point;
    bool m_active;
    bool m_will_be_active;
}


class HypercubeContainer {
public:
    // If a cube already exists at this location, it will be overwritten.
    void add_cube(point_4d point, bool active) {
        m_cubes[point] = new Hypercube(point, active);
    }

    void add_cube(int x, int y, int z, int w, bool active) {
        add_cube(point_4d(x, y, z, w), active);
    }

    Hypercube* get_cube_ptr(int x, int y, int z, int w) {
        return get_cube_ptr(point_4d(x, y, z, w));
    }

    Hypercube* get_cube_ptr(point_4d point) {
        Hypercube* ptr = (point in m_cubes);
        if (ptr is null) {
            m_cubes[point] = new Hypercube(point, false);
            ptr = &m_cubes[point];
            ptr.link_to_neighbors(m_cubes);
        }
        assert (ptr !is null);
        return ptr;
    }

    void initialize_cube_neighbors() {
        foreach (cube; m_cubes) {
            give_cube_neighbors(cube);
            assert(cube.m_neighbors.length == 80);
            // terrible_debug_output_function(0);
        }
    }

    void give_cube_neighbors(ref Hypercube cube) {
        // Generate all the possibile coordinate combos and make them.
        cube.m_neighbors = null;
        foreach (x; cube.point.x - 1 .. cube.point.x + 2)
            foreach (y; cube.point.y - 1 .. cube.point.y + 2)
                foreach(z; cube.point.z - 1 .. cube.point.z + 2)
                    foreach(w; cube.point.w - 1 .. cube.point.w + 2) {
                        if (x == cube.point.x && y == cube.point.y && z == cube.point.z && w == cube.point.w) continue;
                        cube.m_neighbors ~= get_cube_ptr(x, y, z, w);
            }
        assert(cube.m_neighbors.length == 80);
    }

    void update_cubes(int round) {
        int active_cubes = 0;
        foreach (cube; m_cubes) {
            if (cube.check_neighbors(this)) active_cubes += 1;
        }
        foreach (cube; m_cubes) {
            if (cube.m_neighbors.length != 80)
                give_cube_neighbors(cube);
            cube.apply_changes();
        }
        writeln("Round ", round, ": ", active_cubes, " active hypercubes.");
        terrible_debug_output_function(round);
    }

    void terrible_debug_output_function(int round) {
        string filename = "debug" ~ to!string(round) ~ ".txt";
        File file = File(filename, "w");
        foreach(i, cube; m_cubes) {
            string c;
            if (cube.m_active == false)
                c = ".";
            else c = "#";
            file.writeln(c, " ", cube.point.x, " ", cube.point.y, " ", cube.point.z, " ", cube.point.w);
        }
    }

private:
    Hypercube[point_4d] m_cubes;
}

auto parse_input_pt2(string filename) {
    auto file = File(filename);
    char[] buffer;
    string raw_contents;
    while (file.readln(buffer)) {
        raw_contents ~= buffer;
    }

    HypercubeContainer cubes = new HypercubeContainer;

    auto lines = raw_contents.split("\n");
    foreach (x, line; lines) {
        line = line.strip();
        foreach (y, c; line) {
            bool active;
            switch (c) {
                case '.':
                    active = false;
                    break;
                case '#':
                    active = true;
                    break;
                default:
                    writeln("Unexpected character in input: ", c);
                    active = false;
                    break;
            }
            cubes.add_cube(x, y, 0, 0, active);
        }
    }
    return cubes;
}