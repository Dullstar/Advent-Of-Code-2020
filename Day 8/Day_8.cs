using System;
using System.Collections.Generic;

namespace Day_8
{
    enum Opcode
    {
        acc,
        jmp,
        nop
    }

    struct Instruction
    {
        public Instruction(Opcode op, int arg)
        {
            Opcode = op;
            Arg = arg;
        }
        public Opcode Opcode;
        public int Arg;
    }

    class GameConsole
    {
        public GameConsole()
        {
            Accumulator = 0;
            ProgramCounter = 0;
        }
        public int Accumulator { get; private set; }
        public int ProgramCounter { get; private set; }

        public void ExecuteCommand(in Instruction instruction)
        {
            switch (instruction.Opcode)
            {
                case Opcode.acc:
                    Acc(instruction.Arg);
                    break;
                case Opcode.jmp:
                    Jmp(instruction.Arg);
                    break;
                case Opcode.nop:
                    Nop();
                    break;
            }
        }

        public bool RunAll(Instruction[] code)
        {
            Reset();
            bool[] visited = new bool[code.Length];
            while (ProgramCounter < code.Length)
            {
                if (visited[ProgramCounter]) return false;
                visited[ProgramCounter] = true;
                ExecuteCommand(code[ProgramCounter]);
            }
            return true;
        }

        public void FixCode(Instruction[] code)
        {
            // It didn't allow assigning with foreach... C++ allows it with range-based for using non-const ref...
            for (int i = 0; i < code.Length; i++)
            {
                Opcode orig = code[i].Opcode;
                switch(code[i].Opcode)
                {
                    case Opcode.jmp:
                        code[i].Opcode = Opcode.nop;
                        break;
                    case Opcode.nop:
                        code[i].Opcode = Opcode.jmp;
                        break;
                    default:
                        continue;
                }
                if (RunAll(code) == true)
                {
                    Console.WriteLine("Original: " + orig + " " + code[i].Arg + "\tReplacement: " + code[i].Opcode + " " + code[i].Arg
                        + "\tFinal Program Counter: " + ProgramCounter);
                    break;
                }
                code[i].Opcode = orig;
            }
        }
        public void Reset()
        {
            Accumulator = 0;
            ProgramCounter = 0;
        }

        private void Acc(int arg)
        {
            Accumulator += arg;
            ++ProgramCounter;
        }

        private void Jmp(int arg)
        {
            ProgramCounter += arg;
        }

        private void Nop()
        {
            ++ProgramCounter;
        }
    }
    class Day_8
    {
        static Instruction[] ParseInput(string filename)
        {
            string raw_contents = System.IO.File.ReadAllText(filename);
            string[] lines = raw_contents.Trim().Split("\n");
            var code = new List<Instruction>();
            foreach (var line in lines)
            {
                var raw_instruction = line.Split(" ");
                int arg;
                Opcode op;
                if (raw_instruction[0] == "acc") op = Opcode.acc;
                else if (raw_instruction[0] == "jmp") op = Opcode.jmp;
                else op = Opcode.nop;
                arg = int.Parse(raw_instruction[1]);
                code.Add(new Instruction(op, arg));
            }

            return code.ToArray();
        }
        static void Main()
        {
            var code = ParseInput("input.txt");
            var console = new GameConsole();
            console.RunAll(code);
            Console.WriteLine("Stopped at program counter " + console.ProgramCounter 
                + " with accumulator at " + console.Accumulator);
            console.FixCode(code);
            Console.WriteLine("Accumulator was at " + console.Accumulator + " when the program terminated.");
        }
    }
}
