#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <climits>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

struct Extremum
{
	unsigned long long int min, max;
};

std::unordered_map<std::string, char> pair_instructions;
std::unordered_map<std::string, unsigned long long int> pair_map;
char last;

void get_input();
void run_step();
Extremum count();

int main()
{

	get_input();
	for(size_t i = 0; i < (PART-1)*30+10; i++)
		run_step();

	Extremum extr = count();

	std::cout << extr.max - extr.min << std::endl;

}

void get_input()
{

	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	std::string line;
	
	std::getline(input_file, line);
	for(int i = 1; i < line.size(); i++)
	{
		auto pair = pair_map.insert({{line[i-1], line[i]}, 1});
		if(!pair.second)
			pair.first->second++;
	}
	last = line[line.size()-1];

	std::getline(input_file, line);

	while(std::getline(input_file, line))
	{
		std::string pair{line[0], line[1]};
		char res = line[6];
		pair_instructions.insert({pair, res});
	}

}

void run_step()
{
	auto temp = pair_map;
	for(auto& pair : pair_map)
	{
		temp.find(pair.first)->second -= pair.second;
		char c = pair_instructions.find(pair.first)->second;
		auto p1 = temp.insert({{pair.first[0], c}, pair.second});
		auto p2 = temp.insert({{c, pair.first[1]}, pair.second});
		if(!p1.second)
			p1.first->second += pair.second;
		if(!p2.second)
			p2.first->second += pair.second;
	}
	pair_map = temp;
}

Extremum count()
{
	unsigned long long int min = ULLONG_MAX, max = 0;

	std::unordered_map<char, unsigned long long int> counts;
	for(auto p : pair_map)
	{
		auto it_p = counts.insert({p.first[0], p.second});
		if(!it_p.second)
			it_p.first->second += p.second;
	}
	auto it_p = counts.insert({last, 1});
	if(!it_p.second)
		it_p.first->second ++;

	for(auto p : counts)
	{
		max = p.second > max ? p.second : max;
		min = p.second < min ? p.second : min;
	}

	return {min, max};
}