#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <limits.h>
#include <algorithm>
#include <utility>
#include <vector>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

/* I know this is the worse line of code I've ever had to write but
Increase this value by one if the answer is too high, reapeat until its the right answer */
#define ATTEMPT 3

#define LENGHT ((EXAMPLE*-1+1)*90+10 + ((EXAMPLE*-1+1)*90+10)*4*(PART-1))

using ushort = unsigned short;

std::array<std::array<ushort, LENGHT>, LENGHT> risk_map;
std::array<std::array<unsigned int, LENGHT>, LENGHT> total_risk_map;

#define DIST_MAX UINT_MAX - 9

void get_input();
void upscale();
void find_path();

int main()
{

	get_input();

	#if PART == 2
	upscale();
	#endif

	/* Again I should be ashamed but I have no knowledge about pathfinding
	For me, 3 iterations over this find_path() function has been enough */
	for(ushort i = 0; i < ATTEMPT; i++)
		find_path();

	std::cout << total_risk_map[LENGHT - 1][LENGHT - 1] << std::endl;

}

void get_input()
{

	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	std::string line;

	for(ushort y = 0; std::getline(input_file, line); y++)
	{
		for(ushort x = 0; x < line.size(); x++)
			risk_map[x][y] = line[x] - '0';
	}

	for(auto& col : total_risk_map)
		col.fill(DIST_MAX);
	total_risk_map[0][0] = 0;

}

void upscale()
{
	for(ushort x = 0; x < LENGHT/5; x++)
	{
		for(ushort y = 0; y < LENGHT/5; y++)
		{
			for (ushort i = 0; i < 5; i++)
			{
				for(ushort j = 0; j < 5; j++)
				{
					ushort value = risk_map[x][y] + i + j;
					risk_map[x + LENGHT/5 * i][y + LENGHT/5 * j] = value >= 10 ? value%10+1 : value;
				}
			}
		}
	}
}

void find_path()
{
	using Position = std::pair<ushort, ushort>;

	for(ushort x = 0; x < LENGHT; x++)
	{
		for(ushort y = 0; y < LENGHT; y++)
		{
			std::vector<Position> neighbours;
			neighbours.reserve(4);
			if (x < (LENGHT-1)) neighbours.push_back({x + 1, y});
        	if (y < (LENGHT-1)) neighbours.push_back({x, y + 1});
        	if (x > 0)        	neighbours.push_back({x - 1, y});
        	if (y > 0)        	neighbours.push_back({x, y - 1});

        	for(const Position& pos : neighbours)
        		total_risk_map[x][y] = std::min(total_risk_map[x][y], total_risk_map[pos.first][pos.second] + risk_map[x][y]);
		}
	}

}