#include <iostream>
#include <fstream>
#include <array>
#include <string>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2 			// define as 1 to sum part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

unsigned short flashes = 0;
size_t step_all_flash = 0;

struct Octopus
{
	unsigned short energy = 0;
	bool have_flashed = false;
};

std::array<std::array<Octopus, 10>, 10> octopus_map;

void get_input();
void run_step(size_t steps);

int main()
{

	get_input();

	#if PART == 1
	run_step(100);
	std::cout << flashes << std::endl;
	#elif PART == 2
	run_step(SIZE_MAX);
	std::cout << step_all_flash << std::endl;
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

	for(unsigned short y = 0; y < 10; y++)
	{
		std::getline(input_file, line);
		for(unsigned short x = 0; x < 10; x++)
			octopus_map[x][y].energy = line[x] - '0';
	}
}

void run_step(size_t steps)
{
	for(size_t step = 0; step < steps; step++)
	{
		for(unsigned short x = 0; x < 10; x++)
		{
			for(unsigned short y = 0; y < 10; y++)
				octopus_map[x][y].energy++;
		}
	
		bool someone_have_flashed = true;
		while(someone_have_flashed)
		{
			someone_have_flashed = false;
			for(unsigned short x = 0; x < 10; x++)
			{
				for(unsigned short y = 0; y < 10; y++)
				{
					Octopus& octopus = octopus_map[x][y];
					if(octopus.energy > 9 && !octopus.have_flashed)
					{
						octopus.have_flashed = true;
						someone_have_flashed = true;
						flashes++;
						if(x != 9)
							octopus_map[x+1][y].energy++;
						if(x != 9 && y != 9)
							octopus_map[x+1][y+1].energy++;
						if(x != 9 && y != 0)
							octopus_map[x+1][y-1].energy++;
						if(y != 9)
							octopus_map[x][y+1].energy++;
						if(y != 0)
							octopus_map[x][y-1].energy++;
						if(x != 0 && y !=9)
							octopus_map[x-1][y+1].energy++;
						if(x != 0 && y != 0)
							octopus_map[x-1][y-1].energy++;
						if(x != 0)
							octopus_map[x-1][y].energy++;
					}
				}
			}
		}

		bool have_all_flashed = true;

		for(unsigned short x = 0; x < 10; x++)
		{
			for(unsigned short y = 0; y < 10; y++)
			{
				Octopus& octopus = octopus_map[x][y];
				if(octopus.have_flashed)
				{
					octopus.have_flashed = false;
					octopus.energy = 0;
				}
				else
					have_all_flashed = false;
			}
		}

		if(have_all_flashed)
		{
			step_all_flash = step + 1;
			#if PART == 2
			break;
			#endif
		}
	}
}