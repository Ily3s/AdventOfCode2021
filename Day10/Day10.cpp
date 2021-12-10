#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2 			// define as 1 to sum part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

unsigned long long int points = 0;

#if PART == 1
std::unordered_map<char, unsigned short> point_table
{
	{']', 57}, {'}', 1197}, {'>', 25137}, {')', 3}
};
#elif PART == 2
std::unordered_map<char, unsigned short> point_table
{
	{']', 2}, {'}', 3}, {'>', 4}, {')', 1}
};
#endif

#if PART == 2
std::vector<unsigned long long int> scores;
#endif

int main()
{

	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	std::string line;
	while(std::getline(input_file, line))
	{
		std::string expected;
		bool is_corrupted = false;

		for(char c : line)
		{
			if(c == '[')
				expected.push_back(']');
			else if(c == '{')
				expected.push_back('}');
			else if(c == '<')
				expected.push_back('>');
			else if(c == '(')
				expected.push_back(')');
			else if(c == expected.back())
				expected.pop_back();
			else
			{
				#if PART == 1
				points += point_table.find(c)->second;
				#endif
				is_corrupted = true;
				break;
			}
		}

		#if PART == 2

		if(is_corrupted)
			continue;

		points = 0;

		std::reverse(expected.begin(), expected.end());
		for(char c : expected)
			points = points * 5 + point_table.find(c)->second;

		scores.push_back(points);

		#endif
	}

	#if PART == 2
	std::sort(scores.begin(), scores.end());
	points = scores[scores.size()/2];
	#endif

	std::cout << points << std::endl;

}