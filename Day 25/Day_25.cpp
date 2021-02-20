#include <vector>
#include <string>
#include <cinttypes>
#include <iostream>
#include <fstream>
#include "timer.h"

class Device
{
public:
	int32_t loop_size;
	int32_t public_key;
	Device(int32_t _public_key, int32_t _loop_size)
		: public_key(_public_key), loop_size(_loop_size) {}

	int64_t transform(int32_t subject_number) const
	{
		constexpr int32_t magic_number = 20201227;
		int64_t value = 1;
		for (int i = 0; i < loop_size; ++i) {
			value *= subject_number;
			value %= magic_number;
		}
		return value;
	}
};

std::vector<Device> parse_input(const char* filename)
{
	std::ifstream file(filename);
	std::string str1, str2;
	std::getline(file, str1);
	std::getline(file, str2);
	int32_t device1 = std::stoi(str1);
	int32_t device2 = std::stoi(str2);
	std::vector<Device> devices;
	int64_t value = 1;
	constexpr int32_t subject_number = 7;
	constexpr int32_t magic_number = 20201227;
	int32_t i = 0;
	while (devices.size() < 2)
	{
		value *= subject_number;
		value %= magic_number;
		i++;
		if (value == device1) {
			devices.emplace_back(Device(device1, i));
		}
		else if (value == device2) {
			devices.emplace_back(Device(device2, i));
		}
	}
	return devices;
}

int main()
{
	Timer timer;
	auto devices = parse_input("input.txt");
	std::cout << "Encryption Key (using card): " << devices[0].transform(devices[1].public_key) << "\n";
	std::cout << "Encryption Key (using door): " << devices[1].transform(devices[0].public_key) << "\n";
}