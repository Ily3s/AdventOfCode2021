#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#define PART 2

int main()
{

	std::vector<std::string> submarine_input;
	std::ifstream input_file{"input.txt"};
	std::string line;
	while(std::getline(input_file, line))
		submarine_input.push_back(line);

	int depth = 0;
	unsigned int hor_pos = 0;
	int aim = 0;

	for (std::string& str : submarine_input)
	{
		auto space = str.find(' ');
		std::string cmd{str.begin(), str.begin() + space};
		if (cmd == "forward")
		{
			int x = std::stoi(std::string(str.begin() + space, str.end()));
			hor_pos += x;
			#if PART == 2
			depth += aim * x;
			#endif
		}
		else if (cmd == "up")
			#if PART == 1
			depth -= std::stoi(std::string(str.begin() + space, str.end()));
			#elif PART == 2
			aim -= std::stoi(std::string(str.begin() + space, str.end()));
			#endif
		else
			#if PART == 1
			depth += std::stoi(std::string(str.begin() + space, str.end()));
			#elif PART == 2
			aim += std::stoi(std::string(str.begin() + space, str.end()));
			#endif
	}

	std::cout << depth * hor_pos << std::endl;

	return 0;
}