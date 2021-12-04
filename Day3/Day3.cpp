#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <fstream>

#define PART 2
#define SIZE 12

int main()
{

std::vector<std::bitset<SIZE>> input;
std::ifstream input_file{"input.txt"};
std::string line;
while (std::getline(input_file, line))
	input.push_back(std::stoi(line, nullptr, 2));

#if PART == 1

	std::bitset<SIZE> gamma_rate;
	std::bitset<SIZE> eupsilon_rate;

	for (int i = 0; i < SIZE; i++)
	{
		int zer_count = 0;

		for (std::bitset<SIZE> nb : input)
		{
			if (nb[i] == 0)
				zer_count++;
		}

		if (zer_count > input.size() / 2)
		{
			gamma_rate.reset(i);
			eupsilon_rate.set(i);
		}
		else
		{
			gamma_rate.set(i);
			eupsilon_rate.reset(i);
		}
	}

	std::cout << gamma_rate.to_ulong() * eupsilon_rate.to_ulong() << std::endl;

#endif

#if PART == 2

	std::vector<std::bitset<SIZE>> oxygen_vec = input;

	for(int i = 0; oxygen_vec.size() > 1 && i < SIZE; i++)
	{
		bool most_bit = true;
		int zer_count = 0;

		for (std::bitset<SIZE> nb : oxygen_vec)
		{
			if (nb[SIZE-1 - i] == 0)
				zer_count++;
		}

		if (zer_count > oxygen_vec.size() - zer_count)
			most_bit = false;

		for(int index = 0; index < oxygen_vec.size(); index++)
		{
			if(oxygen_vec.size() == 1)
				break;
			if(oxygen_vec[index][SIZE-1 - i] != most_bit)
			{
				oxygen_vec.erase(oxygen_vec.begin() + index);
				index--;
			}
		}
	}

	std::vector<std::bitset<SIZE>> co2_vec = input;

	for(int i = 0; co2_vec.size() > 1 && i < SIZE; i++)
	{
		bool least_bit = false;
		int zer_count = 0;

		for (std::bitset<SIZE> nb : co2_vec)
		{
			if (nb[SIZE-1 - i] == 0)
				zer_count++;
		}

		if (zer_count > co2_vec.size() - zer_count)
			least_bit = true;

		for(int index = 0; index < co2_vec.size(); index++)
		{
			if(co2_vec.size() == 1)
				break;
			if(co2_vec[index][SIZE-1 - i] != least_bit)
			{
				co2_vec.erase(co2_vec.begin() + index);
				index--;
			}
		}
	}

	std::cout << oxygen_vec[0].to_ulong() * co2_vec[0].to_ulong() << std::endl;

#endif

	return 0;
}