#include <iostream>
#include <vector>

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

// defining the input file
#if EXAMPLE == 1
#define IN_FILE "example.txt"
#elif EXAMPLE == 0
#define IN_FILE "input.txt"
#endif

// A list of lanternfishes's timers/grades stored as chars not to waste memory
unsigned char lanternfishes[] = {
#include IN_FILE
};

// The numbers of fishes in each grade of the school
unsigned long long school[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// transfer data from lanternfishes to school
void build_school();

// updates school to the next day
void new_day();

int main()
{

	build_school();

	for(short s = 0; s < DAY; s++)
		new_day();

	unsigned long long total = 0;
	for(const unsigned long long& l : school)
		total += l;

	std::cout << total << std::endl;

}

void build_school()
{
	for(unsigned short s = 0; s < sizeof(lanternfishes); s++)
		school[lanternfishes[s]]++;
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