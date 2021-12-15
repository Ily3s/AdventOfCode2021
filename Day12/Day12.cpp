#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2 			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

void get_input();

std::unordered_map<std::string, std::vector<const std::string*>> connexions;

unsigned int paths = 0;

std::vector<std::string> passed_through{"start"};
const std::string* exception;

void explore_path(const std::string& cave);

int main()
{

	get_input();

	auto start = connexions.find("start");
	for(const std::string* cave_ptr : start->second)
	{
		const std::string& cave = *cave_ptr;
		if(cave == "end")
		{
			paths++;
			continue;
		}
		bool is_upper = std::find_if(cave.begin(), cave.end(), [](char c){return std::isupper(c);}) != cave.end();
		if(!is_upper)
			passed_through.push_back(cave);
		explore_path(cave);
	}

	std::cout << paths << std::endl;

}

void explore_path(const std::string& cave)
{
	auto cave_it = connexions.find(cave);
	for(const std::string* cave_2nd_ptr : cave_it->second)
	{
		const std::string& cave_2nd = *cave_2nd_ptr;
		bool is_upper = std::find_if(cave_2nd.begin(), cave_2nd.end(), [](char c){return std::isupper(c);}) != cave_2nd.end();

		if(!is_upper)
		{
			if(std::find(passed_through.begin(), passed_through.end(), cave_2nd) != passed_through.end())
			{
				#if PART == 2
				if(!exception && cave_2nd != "start" && cave_2nd != "end")
					exception = &cave_2nd;
				else
				#endif
				continue;
			}
			if(cave_2nd != "end")
				passed_through.push_back(cave_2nd);
		}
		if(cave_2nd == "end")
		{
			paths++;
			continue;
		}
		explore_path(cave_2nd);
	}
	auto it = std::find(passed_through.begin(), passed_through.end(), cave);
	if(it != passed_through.end())
		passed_through.erase(it);
	if(exception == &cave)
		exception = nullptr;
}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	std::string line;

	connexions.reserve(25);

	while(std::getline(input_file, line))
	{
		std::string cave1{line.begin(), line.begin() + line.find('-')};
		std::string cave2{line.begin() + line.find('-') + 1, line.end()};

		auto it1 = connexions.find(cave1);
		auto it2 = connexions.find(cave2);
		if(it1 == connexions.end())
			connexions.insert({cave1, {}});
		if(it2 == connexions.end())
			connexions.insert({cave2, {}});
		it1 = connexions.find(cave1);
		it2 = connexions.find(cave2);
		it1->second.push_back(&(it2->first));
		it2->second.push_back(&(it1->first));
	}
}