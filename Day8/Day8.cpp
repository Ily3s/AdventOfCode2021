#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 1 			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

std::vector<std::string> input_vector;
char possibilities[5040][7];

void get_input();
void build_possibilities();

int main()
{

	get_input();

	#if PART == 1

	unsigned int count = 0;
	for(const std::string& str : input_vector)
	{
		std::string digits{str.cbegin() + str.find('|') + 2, str.cend()};
		for(short i = 0; i < 3; i++)
		{
			std::string digit{digits.begin(), digits.begin() + digits.find(' ')};
			if(digit.size() <= 4 || digit.size() == 7)
				count++;
			digits.erase(digits.begin(), digits.begin() + digits.find(' ') + 1);
		}
		std::string digit{digits.begin(), digits.end()};
			if(digit.size() <= 4 || digit.size() == 7)
				count++;
	}

	std::cout << count << std::endl;

	#elif PART == 2

	build_possibilities();

	unsigned int output = 0;

	for(const std::string& str : input_vector)
	{
		std::string uncoder{str.cbegin(), str.cbegin() + str.find('|')};
		std::string copy = uncoder;
		std::string digits{str.cbegin() + str.find('|') + 2, str.cend()};
		digits.push_back(' ');

		std::string one, four, seven, eight;
		std::vector<std::string> sixties;
		std::vector<std::string> fifties;

		for(short i = 0; i < 10; i++)
		{
			std::string digit{copy.begin(), copy.begin() + copy.find(' ')};

			if(digit.size() == 2)
				one = digit;
			else if(digit.size() == 3)
				seven = digit;
			else if(digit.size() == 4)
				four = digit;
			else if(digit.size() == 7)
				seven = digit;
			else if(digit.size() == 6)
				sixties.push_back(digit);
			else
				fifties.push_back(digit);

			copy.erase(copy.begin(), copy.begin() + copy.find(' ') + 1);
		}

		for(unsigned short i = 0; i < 5040; i++)
		{
			#define a possibilities[i][0]
			#define b possibilities[i][1]
			#define c possibilities[i][2]
			#define d possibilities[i][3]
			#define e possibilities[i][4]
			#define f possibilities[i][5]
			#define g possibilities[i][6]
			#define N_FIND(var, seg) var.find(seg) == -1
			#define FIND(var, seg) !N_FIND(var, seg)

			if(N_FIND(seven, a) || N_FIND(seven, c) || N_FIND(seven, f))
				continue;
			if(FIND(four, a) || FIND(four, e) || FIND(four, g))
				continue;
			if(N_FIND(one, c) || N_FIND(one, f))
				continue;

			bool zer = false, six = false, nine = false;
			for(const auto& six_len : sixties)
			{
				if(N_FIND(six_len, d))
					zer = true;
				else if(N_FIND(six_len, e))
					nine = true;
				else if(N_FIND(six_len, c))
					six = true;
			}
			if(!(zer && six && nine))
				continue;

			bool five = false, two = false, three = false;
			for(const auto& fiv_len : fifties)
			{
				if(N_FIND(fiv_len, c) && N_FIND(fiv_len, e))
					five = true;
				else if(N_FIND(fiv_len, b) && N_FIND(fiv_len, f))
					two = true;
				else if(N_FIND(fiv_len, b) && N_FIND(fiv_len, e))
					three = true;
			}

			if(!(five && two && three))
				continue;

			uncoder = {possibilities[i], 7};
			break;
		}

		for(short i = 0; i < 4; i++)
		{
			std::string digit{digits.begin(), digits.begin() + digits.find(' ')};

			if(digit.size() == 2)
				output += std::pow(10, 3-i);
			else if(digit.size() == 3)
				output += 7 * std::pow(10, 3-i);
			else if(digit.size() == 4)
				output += 4 * std::pow(10, 3-i);
			else if(digit.size() == 7)
				output += 8 * std::pow(10, 3-i);
			else if(digit.size() == 6)
			{
				if(N_FIND(digit, uncoder[4]))
					output += 9 * std::pow(10, 3-i);
				else if(N_FIND(digit, uncoder[2]))
					output += 6 * std::pow(10, 3-i);
			}
			else if(N_FIND(digit, uncoder[2]))
				output += 5 * std::pow(10, 3-i);
			else if(N_FIND(digit, uncoder[4]))
				output += 3 * std::pow(10, 3-i);
			else
				output += 2 * std::pow(10, 3-i);

			digits.erase(digits.begin(), digits.begin() + digits.find(' ') + 1);
		}

	}

	std::cout << output << std::endl;

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
		input_vector.push_back(line);
}

void build_possibilities()
{
	#define ASSIGN(i, var) possibilities[index][i] = var
	unsigned short index = 0;
	for(char c1 = 'a'; c1 <= 'g'; c1++)
	{
		for(char c2 = 'a'; c2 <= 'g'; c2++)
		{
			if(c2 == c1)
				continue;
			for(char c3 = 'a'; c3 <= 'g'; c3++)
			{
				if(c3 == c1 || c3 == c2)
					continue;
				for(char c4 = 'a'; c4 <= 'g'; c4++)
				{
					if(c4 == c1 || c4 == c2 || c4 == c3)
						continue;
					for(char c5 = 'a'; c5 <= 'g'; c5++)
					{
						if(c5 == c1 || c5 == c2 || c5 == c3 || c5 == c4)
							continue;
						for(char c6 = 'a'; c6 <= 'g'; c6++)
						{
							if(c6 == c1 || c6 == c2 || c6 == c3 || c6 == c4 || c6 == c5)
								continue;
							for(char c7 = 'a'; c7 <= 'g'; c7++)
							{
								if(c7 == c1 || c7 == c2 || c7 == c3 || c7 == c4 || c7 == c5 || c7 == c6)
									continue;
								ASSIGN(0, c1); ASSIGN(1, c2); ASSIGN(2, c3); ASSIGN(3, c4); ASSIGN(4, c5); ASSIGN(5, c6); ASSIGN(6, c7);
								index++;
							}
						}
					}
				}
			}
		}
	}
}