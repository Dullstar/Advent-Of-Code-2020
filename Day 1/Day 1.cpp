#include <string>
#include <iostream>
#include <fstream>

#include <tclap/CmdLine.h>

// Goal: find the 2 values in the input that sum to 2020, then multiply them together.

int find_two(const std::vector<int>& values) {
	for (size_t i = 0; i < values.size(); ++i) {
		for (size_t j = 0; j < values.size(); ++j) {
			// We don't want to compare this value to itself, but we're checking indices in case it's possible for there
			// to be multiple instances of the same value.
			if (i != j && values[i] + values[j] == 2020) {
				int result = values[i] * values[j];
				std::cout << "The result is: " << values[i] << " * " << values[j] << " = " << result << std::endl;
				return result; // It sounds like we're okay to stop once we find any result.
			}
		}
	}
	throw "Find two: No result was found.";
}

int find_three(const std::vector<int>& values) {
	for (size_t i = 0; i < values.size(); ++i) {
		for (size_t j = 0; j < values.size(); ++j) {
			for (size_t k = 0; k < values.size(); ++k) {
				// See find_two for reasoning for this conditional
				if (i != j && j != k && i != k && values[i] + values[j] + values[k] == 2020) {
					int result = values[i] * values[j] * values[k];
					std::cout << "The result is: " << values[i] << " * " << values[j] << " = " << result << std::endl;
					return result; // It sounds like we're okay to stop once we find any result.
				}
			}
		}
	}
	throw "Find three: No result was found.";
}

int main(int argc, char** argv) {
	std::string infile;

	try {
		TCLAP::CmdLine cmd("Advent of Code 2020 - Puzzle 1", ' ', "1.0");

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

	std::vector<int> values;
	while (!inputFile.eof()) {
		int value;
		inputFile >> value;
		values.push_back(value);
	}

	try { 
		find_two(values); 
	}
	catch (const char* error) {
		std::cout << error << std::endl;
	}
	try {
		find_three(values);
	}
	catch (const char* error) {
		std::cout << error << std::endl;
	}
	return 0;
}