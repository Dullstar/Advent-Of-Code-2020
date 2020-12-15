module Day_15;

import std.stdio;
import std.conv;
import std.string;
import std.datetime.stopwatch;

class MemGame {
    int[int] memory;
    int turn_count;
    int previous_num;
    this(int[] numbers) {
        turn_count = 1;
        foreach(number; numbers) {
            memory[number] = turn_count;
            previous_num = number;
            turn_count++;
        }
        memory.remove(previous_num);
    }

    void process_turn() {
        int next;
        int previous_turn = turn_count - 1;
        int* ptr = (previous_num in memory);
        if (ptr is null)
            next = 0;
        else {
            next = previous_turn - *ptr;
        }

        // writeln("Turn ", turn_count, ": ", next);
        memory[previous_num] = previous_turn;
        turn_count++;
        previous_num = next;
    }

    void process_to(int stop) {
        while (turn_count < stop)
            process_turn();
        writeln("Turn ", turn_count - 1, ": ", previous_num);
    }
}

auto parse_input(string filename) {
    int[] output;
    auto file = File(filename);
    char[] buffer;
    string raw_contents;
    while (file.readln(buffer)) {
        raw_contents ~= buffer;
    }
    auto lines = raw_contents.split(",");
    foreach (line; lines) {
        if (line != "\n")
            output ~= line.strip.to!int;
    }
    return output;
}

void main() {
    //auto sw = StopWatch(AutoStart.yes);
    auto start = MonoTime.currTime;
    auto numbers = parse_input("input.txt");
    auto game = new MemGame(numbers);
    game.process_to(2021);
    game.process_to(30000001);
    writeln((MonoTime.currTime - start));
    //sw.stop();
    //writeln(sw.peek);
}
