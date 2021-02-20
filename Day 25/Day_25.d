module Day_25;

import std.stdio;
import std.string;
import std.conv;
import core.time;

class Device
{
public:
    int loop_size = 0;
    int public_key;
    this(int _public_key, int _loop_size)
    {
        public_key = _public_key;
        loop_size = _loop_size;
    }
    
    int transform(int subject_number) const
    {
        // writeln("Transforming: loop = ", loop_size, " number = ", subject_number);
        const int magic_number = 20201227;
        long value = 1;
        foreach (int i; 0..loop_size) {
            value *= subject_number;
            value %= magic_number;
        }
        return value.to!int;
    }
}

Device[] parse_input(string filename) {
    auto file = File(filename);
    string line1 = file.readln.strip;
    string line2 = file.readln.strip;
    int key1 = line1.to!int;
    int key2 = line2.to!int;
    Device[] devices;
    const int magic_number = 20201227;
    const int subject_number = 7;
    long value = 1;
    int i = 0;
    while (devices.length < 2) {
        value *= subject_number;
        value %= magic_number;
        i++;
        if (value == key1) {
            devices ~= new Device(key1, i);
        }
        else if (value == key2) {
            devices ~= new Device(key2, i);
        }
    }
    return devices;
}

void main()
{
    auto start_time = MonoTime.currTime;
    auto devices = parse_input("input.txt");
    writeln("Encryption Key (using card): ", devices[0].transform(devices[1].public_key));
    writeln("Encryption Key (using key):  ", devices[1].transform(devices[0].public_key));
    auto end_time = MonoTime.currTime;
    writeln("Elapsed time: ", float((end_time - start_time).total!"usecs") / 1000, " ms");
}
