#include <iostream>
#include <algorithm>
#include <functional>
#include <array>
#include <climits>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2 			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

// defining the input file
#if EXAMPLE == 1
#define IN_FILE "example.txt"
#define SIZE_POS 10
#elif EXAMPLE == 0
#define IN_FILE "input.txt"
#define SIZE_POS 1000
#endif

using ushort = unsigned short;
std::array<ushort, SIZE_POS> positions = {
	#include IN_FILE
};

int main()
{
	unsigned int fuel = 0;

	#if PART == 1

	std::sort(positions.begin(), positions.end(), [](ushort a, ushort b){return a < b;});
	ushort n = positions.size() / 2;
	ushort median = (ushort)((positions[n] + positions[n-1]) / 2);

	for(ushort pos : positions)
		fuel += std::abs((short)(pos - median));

	std::cout << fuel << std::endl;

	#elif PART == 2

	unsigned int fuel_min = UINT_MAX;
	unsigned int max_x = *std::max_element(positions.begin(), positions.end());
	for(ushort us = 0; us <= max_x; us++)
	{
		fuel = 0;
		for(ushort pos : positions)
		{
			auto dist = std::abs((short)(pos - us));
			fuel += dist * (dist + 1) / 2;
		}
		if(fuel < fuel_min)
			fuel_min = fuel;
	}

	std::cout << fuel_min << std::endl;

	#endif
}