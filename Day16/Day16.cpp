#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <algorithm>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

using ushort = unsigned short;
using ull_int = unsigned long long int;

std::string bin;
unsigned int version_sum = 0;

void get_input();
const char* hex_to_bin(char c);

ull_int parse_packet();

int main()
{

	get_input();
	ull_int value = parse_packet();

	#if PART == 1
	std::cout << version_sum << std::endl;
	#elif PART == 2
	std::cout << value << std::endl;
	#endif

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{ "example.txt" };
	#elif EXAMPLE == 0
	std::ifstream input_file{ "input.txt" };
	#endif

	std::string line;
	std::getline(input_file, line);

	for (char c : line)
		bin.append(hex_to_bin(c));
}

const char* hex_to_bin(char c)
{
	switch (c)
	{
	case '0': return "0000";
	case '1': return "0001";
	case '2': return "0010";
	case '3': return "0011";
	case '4': return "0100";
	case '5': return "0101";
	case '6': return "0110";
	case '7': return "0111";
	case '8': return "1000";
	case '9': return "1001";
	case 'A': return "1010";
	case 'B': return "1011";
	case 'C': return "1100";
	case 'D': return "1101";
	case 'E': return "1110";
	case 'F': return "1111";
	}
}

ull_int parse_packet()
{
	std::string version_str{ bin.begin(), bin.begin() + 3 };
	std::string type_id_str{ bin.begin() + 3, bin.begin() + 6 };
	bin.erase(bin.begin(), bin.begin() + 6);

	ushort version = std::stoi(version_str, nullptr, 2);
	ushort type_id = std::stoi(type_id_str, nullptr, 2);

	version_sum += version;

	if (type_id == 4)
	{
		bool not_ended = true;
		std::string value_str;
		while (not_ended)
		{
			if (bin.front() == '0')
				not_ended = false;
			bin.erase(bin.begin());

			value_str.append({ bin.begin(), bin.begin() + 4 });
			bin.erase(bin.begin(), bin.begin() + 4);
		}
		return std::stoll(value_str, nullptr, 2);
	}
	else
	{

		std::vector<ull_int> sub_packets_values;

		char lenght_type_id{ bin.front() };
		bin.erase(bin.begin());
		if (lenght_type_id == '0')
		{
			std::string lenght_str{ bin.begin(), bin.begin() + 15 };
			bin.erase(bin.begin(), bin.begin() + 15);
			ushort lenght = std::stoi(lenght_str, nullptr, 2);

			size_t curr_size = bin.size();
			while (bin.size() > curr_size - lenght)
				sub_packets_values.push_back(parse_packet());
		}
		else
		{
			std::string packet_nb_str{ bin.begin(), bin.begin() + 11 };
			bin.erase(bin.begin(), bin.begin() + 11);
			ushort packet_nb = std::stoi(packet_nb_str, nullptr, 2);

			for (ushort i = 0; i < packet_nb; i++)
				sub_packets_values.push_back(parse_packet());
		}

		if (type_id == 0)
		{
			ull_int value = 0;
			for (auto& i : sub_packets_values)
				value += i;
			return value;
		}
		else if (type_id == 1)
		{
			ull_int value = 1;
			for (auto& i : sub_packets_values)
				value *= i;
			return value;
		}
		else if (type_id == 2)
			return *std::min_element(sub_packets_values.begin(), sub_packets_values.end());
		else if (type_id == 3)
			return *std::max_element(sub_packets_values.begin(), sub_packets_values.end());
		else if (type_id == 5)
			return (ull_int)(sub_packets_values[0] > sub_packets_values[1]);
		else if (type_id == 6)
			return (ull_int)(sub_packets_values[0] < sub_packets_values[1]);
		else if (type_id == 7)
			return (ull_int)(sub_packets_values[0] == sub_packets_values[1]);
	}
}