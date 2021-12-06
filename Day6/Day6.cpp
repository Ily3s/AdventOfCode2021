#include <iostream>
#include <vector>
#include <fstream>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2 			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

// defining the day number knowing the part
#if PART == 1
#define DAY 80
#elif PART == 2
#define DAY 256
#endif

// A list of lanternfishes's timers/grades stored as chars not to waste memory
std::vector<unsigned char> lanternfishes;

// The numbers of fishes in each grade of the school
unsigned long long school[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// transfer data from "input.txt" or "example.txt" to lanterfishes
void get_input();

// transfer data from lanternfishes to school
void build_school();

// updates school to the next day
void new_day();

int main()
{

	get_input();
	build_school();

	for(short s = 0; s < DAY; s++)
		new_day();

	unsigned long long total = 0;
	for(const unsigned long long& l : school)
		total += l;

	std::cout << total << std::endl;

}

void get_input()
{

	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	char c;
	bool b = true;

	while(!input_file.eof())
	{
		if(b)
		{
			input_file.get(c);
			lanternfishes.push_back(c - '0');
			b = false;
		}
		else
		{
			input_file.get();
			b = true;
		}
	}

}

void build_school()
{
	for(unsigned char fish : lanternfishes)
		school[fish]++;
}

void new_day()
{
	// storing the fishes that need to go grade 8 again
	unsigned long long temp = school[0];

	for(unsigned short index = 0; index < 8; index++)
		school[index] = school[index+1];	// each fish goes 1 grade down

	// adding childs of garde 0 fishes to grade 6
	school[6] += temp;

	// immortals grade 0 goes to grade 8 again
	school[8] = temp;
}