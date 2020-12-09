#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "stringsplit.h"

// used to allow us to package both part 1 and part 2 in one function while still being easy to read
enum class valid {
	valid,
	partial,
	invalid
};

class Passport {
public:
	Passport(std::string& raw_fields) {
		raw_fields = strip(raw_fields);
		auto fields = split(raw_fields, " ");
		for (auto& field : fields) {
			field = strip(field);
			auto temp = split(field, ":");
			auto& label = temp[0];
			auto& value = temp[1];
			// Unfortunately I don't think we can do a switch case on a string, so if/if else/if else... it is!
			
			// birth year, issue year, expiration year
			if (label == "byr")
				byr = std::stoi(value);
			else if (label == "iyr")
				iyr = std::stoi(value);
			else if (label == "eyr")
				eyr = std::stoi(value);

			// height - must separate unit from number!
			else if (label == "hgt") {
				std::string number = "";
				std::string unit = "";
				bool adding_to_num = true;
				for (const auto& c : value) {
					if (adding_to_num && std::isdigit(c)) {
						number += c;
						continue;
					}
					else {
						adding_to_num = false;
						unit += c;
					}
				}
				if (number.size() > 0) hgt = std::stoi(number);
				hgt_unit = unit;
			}

			// Hair color, eye color, passport id
			else if (label == "hcl")
				hcl = value;
			else if (label == "ecl")
				ecl = value;
			else if (label == "pid")
				pid = value;
			else if (label == "cid");  // don't care, just don't warn about it
			else std::cout << "Warning: unexpected label found.\n";
		}
	}

	valid validate(std::ofstream* file = nullptr) const {
		if (!check_for_missing_entries()) {
			std::cout << "\tRejected: missing entries.\n\n";
			return valid::invalid;
		}
		else if (!check_for_invalid_entries()) {
			std::cout << "\tRejected: invalid entries.\n\n";
			return valid::partial;
		}
		std::cout << "Accepted.\n\n";
		if (file != nullptr) output_to_file(*file);
		return valid::valid;
	}

private:
	int byr = -1;
	int iyr = -1;
	int eyr = -1;
	int hgt = -1;
	std::string hgt_unit = "";
	std::string hcl = "";
	std::string ecl = "";
	// pid can be represented as a number, but the way we're using it it's more of a string
	std::string pid = "";

	// Debugging helper function.
	[[maybe_unused]] void output_to_file(std::ofstream& file) const {
		file << "byr:" << byr << " iyr:" << iyr << " eyr:" << eyr << " ecl:" << ecl << " hgt:" << hgt << hgt_unit
			<< " hcl:" << hcl << " pid:" << pid << "\n\n";
	}

	bool check_for_missing_entries() const {
		if (byr == -1) {
			std::cout << "Rejected due to missing byr entry.\n";
			return false;
		}
		if (iyr == -1) { 
			std::cout << "Rejected due to missing iyr entry.\n";
			return false;
		}
		if (eyr == -1) {
			std::cout << "Rejected due to missing eyr entry.\n";
			return false;
		}
		if (hgt == -1) {
			std::cout << "Rejected due to missing hgt entry.\n";
			return false;
		}
		if (hcl == "") {
			std::cout << "Rejected due to missing hcl entry.\n";
			return false;
		}
		if (ecl == "") {
			std::cout << "Rejected due to missing ecl entry.\n";
			return false;
		}
		if (pid == "") {
			std::cout << "Rejected due to missing pid entry.\n";
			return false;
		}
		return true;
	}

	bool check_for_invalid_entries() const {
		// endpoints included
		auto entry_between = [&](int min, int max, int val) {
			if (val < min || val > max) return false;
			return true;
		};

		// byr, iyr, eyr are straightforward
		if (!entry_between(1920, 2002, byr)) {
			printf("Rejected: byr %d is not between %d and %d\n", byr, 1920, 2002);
			return false;
		}
		if (!entry_between(2010, 2020, iyr)) {
			printf("Rejected: iyr %d is not between %d and %d\n", iyr, 2010, 2020);
			return false;
		}
		if (!entry_between(2020, 2030, eyr)) {
			printf("Rejected: eyr %d is not between %d and %d\n", byr, 2020, 2030);
			return false;
		}
		
		// hgt requires some checks first
		if (pid == "980577052") {
			std::cout << "STOP\n";
		}
		if (hgt_unit == "in") {
			if (!entry_between(59, 76, hgt)) {
				printf("Rejected: hgt %d in is not between %d and %d\n", hgt, 59, 76);
				return false;
			}
		}
		else if (hgt_unit == "cm") {
			if (!entry_between(150, 193, hgt)) {
				printf("Rejected: hgt %d cm is not between %d and %d\n", hgt, 159, 193);
				return false;
			}
		}
		else {
			std::cout << "Rejected: " << hgt_unit << " is not either in or cm.\n";
			return false;
		}
		// hcl
		if (hcl.size() != 7 || hcl[0] != '#') {
			std::cout << "Rejected: hcl " << hcl << " doesn't conform to expected format #nnnnnn where n is any hex digit\n";
			return false;
		}
		for (int i = 1; i < static_cast<int>(hcl.size()); i++) {
			if (!std::isxdigit(hcl[i])) {
				std::cout << "Rejected: hcl " << hcl << " doesn't conform to expected format #nnnnnn where n is any hex digit\n";
				return false;
			}
		}

		// ecl
		if (ecl != "amb" && ecl != "blu" && ecl != "brn" && ecl != "gry" && ecl != "grn" && ecl != "hzl" && ecl != "oth") {
			std::cout << "Rejected: ecl " << ecl << " isn't allowed.\n";
			return false;
		}

		// pid
			if (pid.size() != 9) {
				std::cout << "Rejected: pid " << pid << " doesn't have exactly 9 digits.\n";
				return false;
			}
		for (const auto& c : pid) {
			if (!std::isdigit(c))
			{
				std::cout << "Rejected: pid " << pid << " isn't a number.\n";
				return false;
			}
		}

		// If you made it here, congratulations! It's (close enough to) valid!
		return true;
	}
};

std::vector<Passport> get_passports(std::string filename) {
	// The Passport object itself is responsible for building itself out of the raw list of fields
	// but we do need to seperate them here
	std::ifstream file(filename);
	std::vector<Passport> passports;
	std::stringstream raw_contents;
	raw_contents << file.rdbuf();
	auto raw_entries = split(raw_contents.str(), "\n\n");
	for (auto& entry : raw_entries) {
		// important cleanup so we can split this in the passport constructor
		for (auto& c : entry) {
			if (c == '\n')
				c = ' ';
		}
		passports.emplace_back(Passport(entry));
	}
	return passports;
}

int main() {
	auto passports = get_passports("input.txt");
	int all_entries_present = 0;
	int all_entries_valid = 0;
	std::ofstream valid_entries("output.txt");
	for (const auto& passport : passports) {
		valid status = passport.validate(&valid_entries);
		if (status == valid::valid || status == valid::partial) {
			if (status == valid::valid) all_entries_valid++;
			all_entries_present++;
		}
	}

	std::cout << all_entries_present << " out of " << passports.size() << " passports have all entries present.\n"
		<< all_entries_valid << " out of " << passports.size() << " passports have all entries valid.\n";
}