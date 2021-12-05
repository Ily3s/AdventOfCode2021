#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

#define PART 2
#define EXAMPLE 0

struct GeoVec
{
	unsigned short x1, y1;
	unsigned short x2, y2;
};

#if EXAMPLE == 1
std::array<unsigned short, 100> pt_map;
#elif EXAMPLE == 0
std::array<unsigned short, 1000000> pt_map;
#endif

std::vector<GeoVec> vectors;

void get_input();
void fill_map();
unsigned int count_danger();

int main()
{

	get_input();
	fill_map();

	std::cout << count_danger() << std::endl;

}

void get_input()
{
	#if EXAMPLE == 1
	vectors.reserve(6);
	#elif EXAMPLE == 0
	vectors.reserve(300);
	#endif

	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	std::string line;
	while(std::getline(input_file, line))
	{
		GeoVec vec;
		vec.x1 = std::stoi(std::string(line.begin(), line.begin() + line.find(',')));
		vec.y1 = std::stoi(std::string(line.begin() + line.find(',') + 1, line.begin() + line.find(' ')));
		line.erase(0, line.find(' ') + 4);
		vec.x2 = std::stoi(std::string(line.begin(), line.begin() + line.find(',')));
		vec.y2 = std::stoi(std::string(line.begin() + line.find(',') + 1, line.end()));
		#if PART == 1
		if(vec.x1 == vec.x2 || vec.y1 == vec.y2)
			vectors.push_back(vec);
		#elif PART == 2
		vectors.push_back(vec);
		#endif
	}
}

void fill_map()
{
	#if EXAMPLE == 1
	#define LENGHT 10
	#elif EXAMPLE == 0
	#define LENGHT 1000
	#endif

	for(const GeoVec& vec : vectors)
	{
		const unsigned short& max_x = std::max(vec.x1, vec.x2);
		const unsigned short& min_x = std::min(vec.x1, vec.x2);
		const unsigned short& max_y = std::max(vec.y1, vec.y2);
		const unsigned short& min_y = std::min(vec.y1, vec.y2);

		if(vec.x1 == vec.x2)
		{
			for(unsigned short y = min_y; y <= max_y; y++)
				pt_map[y * LENGHT + vec.x1] ++;
		}
		else if(vec.y1 == vec.y2)
		{
			for(unsigned short x = min_x; x <= max_x; x++)
				pt_map[vec.y1 * LENGHT + x] ++;
		}

		#if PART == 2
		else
		{
			const unsigned short line_lenght = max_x - min_x;

			if((min_x == vec.x1 && min_y == vec.y1) || (min_x == vec.x2 && min_y == vec.y2))
			{
				for(unsigned short pos = 0; pos <= line_lenght; pos++)
					pt_map[(min_y + pos) * LENGHT + min_x + pos] ++;
			}
			else
			{
				for(unsigned short pos = 0; pos <= line_lenght; pos++)
					pt_map[(min_y + pos) * LENGHT + max_x - pos] ++;
			}
		}
		#endif

	}
}

unsigned int count_danger()
{
	unsigned int output = 0;
	for(unsigned short vent : pt_map)
	{
		if(vent >= 2)
			output++;
	}
	return output;
}