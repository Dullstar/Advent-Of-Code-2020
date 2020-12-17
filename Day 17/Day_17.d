module Day_17;

import std.stdio;
import std.string;
import core.time;
import std.conv;
import Day_17_pt2;


struct point_3d {
    int x;
    int y;
    int z;
    this(int _x, int _y, int _z) {
        x = _x;
        y = _y;
        z = _z;
    }
}

class Cube {
public:
    this(int x, int y, int z, bool active) {
        this(point_3d(x, y, z), active);
    }
    this(point_3d point, bool active) {
        m_point = point;
        m_active = active;
    }

    @property bool active() {
        return m_active;
    }

    @property point_3d point() {
        return m_point;
    }
    
    void apply_changes() {
        m_active = m_will_be_active;
    }

    bool check_neighbors(CubeContainer cubes) {
        //File file = File("output.txt", "a");
        //file.writeln("\nChecking neighbors of ", point.x, " ", point.y, " ", point.z);
        //file.writeln("This cube is active? ", m_active);
        int number_neighbors = m_neighbors.length.to!int;
        int active_neighbors = 0;
        foreach(neighbor; m_neighbors) {
            if (neighbor.active) {
                //file.writeln("Active neighbor detected.", neighbor.point.x, " ", neighbor.point.y, " ", neighbor.point.z);
                active_neighbors++;
            }
            //else file.writeln("Inactive neighbor detected.", neighbor.point.x, " ", neighbor.point.y, " ", neighbor.point.z);
        }

        if (m_active) {
            //writeln("I'm checking my neighbors! I'm at ", m_point.x, ",", m_point.y, ",", m_point.z, " and I have ", active_neighbors, " active neighbors!");
            //writeln("But I also have ", m_neighbors.length, " neighbors...");
            if (active_neighbors == 2 || active_neighbors == 3) {
                m_will_be_active = true;
            }
            else m_will_be_active = false;
        }
        else if (!m_active && active_neighbors == 3)
            m_will_be_active = true;
        else m_will_be_active = false;

        if (m_will_be_active) {
            foreach(neighbor; m_neighbors) {
                if (neighbor.m_neighbors.length != 26) {
                    cubes.give_cube_neighbors(*neighbor);
                }
            }
        }

        return m_will_be_active;
    }

    void link_to_neighbors(ref Cube[point_3d] cubes) {
        foreach (x; m_point.x - 1 .. m_point.x + 2)
            foreach (y; m_point.y - 1 .. m_point.y + 2)
                foreach(z; m_point.z - 1 .. m_point.z + 2) {
                    if (x == m_point.x && y == m_point.y && z == m_point.z) continue;
                    // this time, we DON'T want to create new neighbors.
                    Cube* ptr = point_3d(x, y, z) in cubes;
                    if (ptr !is null) m_neighbors ~= ptr;
                }
    }
    
private:
    Cube*[] m_neighbors;
    point_3d m_point;
    bool m_active;
    bool m_will_be_active;
}


// If I recall correctly from the documentation, by virtue of existing in the same module,
// CubeContainer and Cube are automatically equivalent to friend classes in C++.
class CubeContainer {
public:
    // If a cube already exists at this location, it will be overwritten.
    void add_cube(point_3d point, bool active) {
        m_cubes[point] = new Cube(point, active);
    }

    void add_cube(int x, int y, int z, bool active) {
        add_cube(point_3d(x, y, z), active);
    }

    Cube* get_cube_ptr(int x, int y, int z) {
        return get_cube_ptr(point_3d(x, y, z));
    }

    Cube* get_cube_ptr(point_3d point) {
        Cube* ptr = (point in m_cubes);
        if (ptr is null) {
            m_cubes[point] = new Cube(point, false);
            ptr = &m_cubes[point];
            ptr.link_to_neighbors(m_cubes);
        }
        assert (ptr !is null);
        return ptr;
    }
    
    void initialize_cube_neighbors() {
        foreach (cube; m_cubes) {
            give_cube_neighbors(cube);
            assert(cube.m_neighbors.length == 26);
            // terrible_debug_output_function(0);
        }
    }

    void give_cube_neighbors(ref Cube cube) {
    // Generate all the possibile coordinate combos and make them.
    cube.m_neighbors = null;
    foreach (x; cube.point.x - 1 .. cube.point.x + 2)
        foreach (y; cube.point.y - 1 .. cube.point.y + 2)
            foreach(z; cube.point.z - 1 .. cube.point.z + 2) {
                if (x == cube.point.x && y == cube.point.y && z == cube.point.z) continue;
                    cube.m_neighbors ~= get_cube_ptr(x, y, z);
            }
    // assert(cube.m_neighbors.length == 26);
    }

    void update_cubes(int round) {
        int active_cubes = 0;
        foreach (cube; m_cubes) {
            if (cube.check_neighbors(this)) active_cubes += 1;
        }
        foreach (cube; m_cubes) {
            //writeln("Checking a cube! ", cube.m_point.x, " ", cube.m_point.y, " ", cube.m_point.z, " ", cube.m_active);
            // I have no idea why the commented out line below doesn't work, but it doesn't work. It really seems like it should, though.
            // if ((cube.m_active || cube.m_will_be_active) && cube.m_neighbors.length != 26)
            // Because if we *don't* this is really slow. Tbh, I expected it would become an infinite loop, but apparently not.
            //if (cube.m_neighbors.length != 26) {
            //    give_cube_neighbors(cube);
            //}
            cube.apply_changes();
        }
        writeln("Round ", round, ": ", active_cubes, " active cubes.");
        // terrible_debug_output_function(round);
    }

    void terrible_debug_output_function(int round) {
        string filename = "debug" ~ to!string(round) ~ ".txt";
        File file = File(filename, "w");
        foreach(i, cube; m_cubes) {
            string c;
            if (cube.m_active == false)
                c = ".";
            else c = "#";
            file.writeln(c, " ", cube.point.x, " ", cube.point.y, " ", cube.point.z);
        }
    }

private:
    Cube[point_3d] m_cubes;
}


auto parse_input(string filename) {
    auto file = File(filename);
    char[] buffer;
    string raw_contents;
    while (file.readln(buffer)) {
        raw_contents ~= buffer;
    }

    CubeContainer cubes = new CubeContainer;
    
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
            cubes.add_cube(x, y, 0, active);
        }
    }
    return cubes;
}

void main()
{
    auto start_time = MonoTime.currTime;
    const string filename = "test_input.txt";
    auto cubes = parse_input(filename);
    cubes.initialize_cube_neighbors();
    foreach (round; 1 .. 7) {
        cubes.update_cubes(round);
    }
    auto end_time1 = MonoTime.currTime;
    HypercubeContainer cubes2 = parse_input_pt2(filename);
    cubes2.initialize_cube_neighbors();
    writeln();
    foreach (round; 1 .. 7) {
        cubes2.update_cubes(round);
    }
    auto end_time2 = MonoTime.currTime;
    writeln("Elapsed time (pt 1): ", float((end_time1 - start_time).total!"usecs") / 1000, " ms");
    writeln("Elapsed time (pt 2): ", float((end_time2 - end_time1).total!"usecs") / 1000, " ms");
    writeln("Elapsed time (total): ", float((end_time2 - start_time).total!"usecs") / 1000, " ms");
}