#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

struct Dot
{
	unsigned short x, y;

	bool operator==(const Dot& other)
	{
		return x == other.x && y == other.y;
	}
};

enum class Axis
{
	Vertical,
	Horizontal
};

struct Fold
{
	Axis axis;
	unsigned short position;
};

std::vector<Dot> dots;
std::vector<Fold> next_folds;

void get_input();
void fold();

int main()
{
	get_input();

	#if PART == 1

	fold();
	std::cout << dots.size() << std::endl;

	#elif PART == 2

	while(next_folds.size())
		fold();

	std::vector<std::vector<bool>> dot_map;
	unsigned short lenght = 0, width = 0;

	for(auto d : dots)
	{
		width = d.y >= width ? d.y + 1 : width;
		lenght = d.x >= lenght ? d.x + 1 : lenght;
	}

	dot_map.reserve(width);
	for(unsigned short y = 0; y < width; y++)
	{
		dot_map.push_back({});
		dot_map[y].reserve(lenght);
		for(unsigned x = 0; x < lenght; x++)
			dot_map[y].push_back(false);
	}

	for(auto d : dots)
		dot_map[d.y][d.x] = true;

	for(unsigned short y = 0; y < width; y++)
	{
		for (unsigned short x = 0; x < lenght; x++)
		{
			std::cout << (dot_map[y][x] ? '#' : '.');
		}
		std::cout << '\n';
	}

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

	while(std::getline(input_file, line))
	{
		if(!line.size())
			continue;
		if(line[0] != 'f')
		{
			std::string x{line.begin(), line.begin() + line.find(',')};
			std::string y{line.begin() + line.find(',') + 1, line.end()};
			dots.push_back({(unsigned short)std::stoi(x), (unsigned short)std::stoi(y)});
		}
		else
		{
			std::string pos{line.begin() + line.find('=') + 1, line.end()};
			if(line.find('x') != -1)
				next_folds.push_back({Axis::Vertical, (unsigned short)std::stoi(pos)});
			else
				next_folds.push_back({Axis::Horizontal, (unsigned short)std::stoi(pos)});
		}
	}
}

void fold()
{
	auto& fold = next_folds.front();
	size_t size = dots.size();
	for(size_t i = 0; i < size; i++)
	{
		if(fold.axis == Axis::Horizontal && dots[i].y > fold.position)
		{
			Dot dot = {dots[i].x, (unsigned short)(dots[i].y - (dots[i].y - fold.position) * 2)};
			dots.erase(dots.begin() + i);
			auto it = std::find_if(dots.begin(), dots.end(), [&dot](const Dot& d){return dot == d;});
			if(it == dots.end())
				dots.push_back(dot);
			i--; size--;
		}
		else if(fold.axis == Axis::Vertical && dots[i].x > fold.position)
		{
			Dot dot = {(unsigned short)(dots[i].x - (dots[i].x - fold.position) * 2), dots[i].y};
			dots.erase(dots.begin() + i);
			auto it = std::find_if(dots.begin(), dots.end(), [&dot](const Dot& d){return dot == d;});
			if(it == dots.end())
				dots.push_back(dot);
			i--; size--;
		}
	}
	next_folds.erase(next_folds.begin());
}