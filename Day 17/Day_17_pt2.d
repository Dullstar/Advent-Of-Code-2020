module Day_17_pt2;

import std.stdio;
import std.string;
import core.time;
import std.conv;

// Part 1 was quite a disaster to get working. The approach didn't work at all when ported to 4D,
// and I never managed to figure out why.
// So things had to be rewritten here.

// There's probably still some leftover junk lying around, though - should probably try to find
// a tool that works to this language to detect and remove unused stuff!

// No, this organization doesn't really generalize well to n dimensions.

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

    void push_active(HypercubeContainer cubes) {
        if (m_active) {
            int total_neighbors = 0;
            foreach (x; m_point.x - 1 .. m_point.x + 2)
                foreach (y; m_point.y - 1 .. m_point.y + 2)
                    foreach(z; m_point.z - 1 .. m_point.z + 2)
                        foreach(w; m_point.w - 1 .. m_point.w + 2) {
                            if (m_point.x == x && m_point.y == y && m_point.z == z && m_point.w == w) continue;
                            Hypercube* cube = cubes.get_cube_ptr(x, y, z, w);
                            cube.m_active_neighbors++;
                            total_neighbors++;
                        }
        }
    }

    bool is_active() {
        if (m_active) {
            if (m_active_neighbors == 2 || m_active_neighbors == 3) {
                m_active = true;
            }
            else m_active = false;
        }
        else if (m_active_neighbors == 3) m_active = true;
        else m_active = false;

        return m_active;
    }

private:
    Hypercube*[] m_neighbors;
    point_4d m_point;
    bool m_active;
    bool m_will_be_active;
    int m_active_neighbors = 0;
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
        }
        assert (ptr !is null);
        return ptr;
    }


    void update_cubes(int round) {
        int active_cubes = 0;
        foreach (cube; m_cubes) {
            cube.m_active_neighbors = 0;
        }
        foreach (cube; m_cubes) {
            cube.push_active(this);
        }
        foreach (cube; m_cubes) {
            if (cube.is_active()) active_cubes++;
        }
        writeln("Round ", round, ": ", active_cubes, " active cubes.");
        // visualizer_output_function(round);
    }

    // the visualizer is a separate program, but it doesn't have the capability to run the simulation.
    // it was originally created for debugging purposes.
    void visualizer_output_function(int round) {
        string filename = "visualizer" ~ to!string(round) ~ ".txt";
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
    foreach (y, line; lines) {
        line = line.strip();
        foreach (x, c; line) {
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