#include <string>
#include <iostream>
#include <fstream>

#include <tclap/CmdLine.h>

// GOAL: Count the number of passwords that comply with the rule included with them!
// x-y c: 
int main(int argc, char** argv) {
	// I should probably abstract this T-CLAP code out into its own function that can be included in every daily puzzle,
	// instead of just copy/pasting it each day... but for now copy/paste will work.

	// Alternatively, there only seems to be one input file per day so far, so arguably grabbing the filename from the command line is
	// totally overengineering it.
	std::string infile;

	try {
		TCLAP::CmdLine cmd("Advent of Code 2020 - Day 2", ' ', "1.0");

		TCLAP::ValueArg<std::string> infileArg("i", "input", "Input File", true, "input.txt", "string");

		cmd.add(infileArg);

		cmd.parse(argc, argv);

		infile = infileArg.getValue();
	}
	catch (TCLAP::ArgException& error) {
		std::cerr << error.what() << std::endl;
		return -1;
	}

	std::ifstream inputFile;
	try {
		inputFile.open(infile);
		if (!inputFile.is_open()) {
			std::stringstream errormsg;
			errormsg << "Can't open file: " << infile;
			throw (errormsg.str());
		}
	}
	catch (std::string& errormsg) {
		std::cout << errormsg << std::endl;
		return -1;
	}

	// ---Begin Input Processing Here---

	std::string raw_line;
	int passing = 0;
	int passing2 = 0;
	while (std::getline(inputFile, raw_line)) {
		int min = 0;
		int max = 0;
		char letter = ' ';
		std::string password;
		std::stringstream sstream (raw_line);
		std::string temp;
		std::getline(sstream, temp, '-');
		std::stringstream temp_sstream(temp);
		temp_sstream >> min;
		std::getline(sstream, temp, ' ');
		temp_sstream.clear(); // seeking after the next line would probably also work, but I'd need to check syntax and this works
		temp_sstream.str(temp);
		temp_sstream >> max;
		std::getline(sstream, temp, ' ');
		letter = temp[0];
		std::getline(sstream, password, ' ');
		if (min > max) std::swap(min, max); // just in case
		printf("Min %d, Max %d, Letter %c, Password %s\n", min, max, letter, password.c_str());

		// Code for puzzle 1:
		// password should contain at most min and at most max instances of the code
		int instances = 0;
		for (const char& c : password) {
			if (c == letter) ++instances;
		}
		std::cout << "Number of times letter appeared: " << instances << "\n";
		if (instances >= min && instances <= max) ++passing;

		// Code for puzzle 2
		// password should have the given letter in one (but not both) of the positions specified
		int pos1 = min - 1;  // These are the same values as we used for min max in the input, but reinterpreted
		int pos2 = max - 1;  // 1 is subtracted because our strings are indexed from 0, but the input is indexed from 1

		if ((int)password.size() > pos2) {
			printf("pos1 %c, pos2 %c: password is ", password[pos1], password[pos2]);  // newline intentionally omitted
			if ((password[pos1] == letter) != (password[pos2] == letter)) {
				++passing2;
				std::cout << "valid\n\n";
			}
			else std::cout << "not valid\n\n";
		}

		
	}

	std::cout << "Number of passing passwords (Puzzle 1): " << passing << "\n";
	std::cout << "Number of passing passwords (Puzzle 2): " << passing2 << "\n";
	return 0;
}