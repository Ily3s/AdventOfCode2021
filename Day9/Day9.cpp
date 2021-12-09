#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2 			// define as 1 to sum part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

#if EXAMPLE == 1
#define LENGHT 10
#define WIDTH 5
#elif EXAMPLE == 0
#define LENGHT 100
#define WIDTH 100
#endif

struct PointStorage
{
	unsigned short x, y;
};

struct PointBassine
{
	unsigned short height;
	bool is_in_bassine = false;
};

std::array<std::array<PointBassine, WIDTH>, LENGHT> heightmap;

void get_input();

std::vector<PointStorage> weak_points;

std::vector<unsigned short> bassines;

void get_flowing(unsigned short x, unsigned short y, unsigned short bassine_index);

int main()
{

	get_input();

	unsigned int risk_level;

	for(size_t x = 0; x < LENGHT; x++)
	{
		for(size_t y = 0; y < WIDTH; y++)
		{
			if(x != LENGHT-1 && heightmap[x+1][y].height <= heightmap[x][y].height)
				continue;
			if(x && heightmap[x-1][y].height <= heightmap[x][y].height)
				continue;
			if(y != WIDTH-1 && heightmap[x][y+1].height <= heightmap[x][y].height)
				continue;
			if(y && heightmap[x][y-1].height <= heightmap[x][y].height)
				continue;

			risk_level += heightmap[x][y].height + 1;
			weak_points.push_back({(unsigned short)x, (unsigned short)y});
		}
	}

	#if PART == 1

	std::cout << risk_level << std::endl;

	#elif PART == 2

	unsigned short bassine_first;
	unsigned short bassine_second;
	unsigned short bassine_third;

	bassines.reserve(weak_points.size());

	for(size_t index = 0; index < weak_points.size(); index++)
	{
		auto& p = weak_points[index];
		bassines.push_back(1);
		heightmap[p.x][p.y].is_in_bassine = true;
		get_flowing(p.x, p.y, index);
	}

	auto it = std::max_element(bassines.begin(), bassines.end());
	bassine_first = *it;
	bassines.erase(it);
	it = std::max_element(bassines.begin(), bassines.end());
	bassine_second = *it;
	bassines.erase(it);
	it = std::max_element(bassines.begin(), bassines.end());
	bassine_third = *it;

	std::cout << bassine_first * bassine_second * bassine_third << std::endl;

	#endif

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	std::string line;
	size_t y = 0;
	while(std::getline(input_file, line))
	{
		for(size_t x = 0; x < line.size(); x++)
			heightmap[x][y].height = line[x] - '0';
		y++;
	}
}

void get_flowing(unsigned short x, unsigned short y, unsigned short bassine_index)
{
	PointBassine& p = heightmap[x][y];
	if(x != LENGHT-1 && !heightmap[x+1][y].is_in_bassine && heightmap[x+1][y].height != 9)
	{
		bassines[bassine_index] ++;
		heightmap[x+1][y].is_in_bassine = true;
		get_flowing(x+1, y, bassine_index);
	}
	if(x && !heightmap[x-1][y].is_in_bassine && heightmap[x-1][y].height != 9)
	{
		bassines[bassine_index] ++;
		heightmap[x-1][y].is_in_bassine = true;
		get_flowing(x-1, y, bassine_index);
	}
	if(y != WIDTH-1 && !heightmap[x][y+1].is_in_bassine && heightmap[x][y+1].height != 9)
	{
		bassines[bassine_index] ++;
		heightmap[x][y+1].is_in_bassine = true;
		get_flowing(x, y+1, bassine_index);
	}
	if(y && !heightmap[x][y-1].is_in_bassine && heightmap[x][y-1].height != 9)
	{
		bassines[bassine_index] ++;
		heightmap[x][y-1].is_in_bassine = true;
		get_flowing(x, y-1, bassine_index);
	}
}