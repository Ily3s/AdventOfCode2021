#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define PART 2

int main()
{

	std::vector<short> depths;
	std::ifstream input_file{"input.txt"};
	std::string line;
	while(std::getline(input_file, line))
		depths.push_back((short)std::stoi(line));

	size_t increase_nb = 0;

#if PART == 1

	for (size_t index = 1; index < depths.size(); index++)
		if(depths[index] > depths[index-1]) increase_nb++;

	std::cout << increase_nb << std::endl;

#elif PART == 2

	increase_nb = 0;

	std::vector<short> three_measurments;
	three_measurments.reserve(depths.size());

	for (size_t index = 0; index < depths.size() - 2; index++)
		three_measurments.push_back(depths[index] + depths[index + 1] + depths[index + 2]);

	for (size_t index = 1; index < three_measurments.size(); index++)
		if(three_measurments[index] > three_measurments[index-1]) increase_nb++;

	std::cout << increase_nb << std::endl;

#endif

}