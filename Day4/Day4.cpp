#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>

#define PART 2

struct Nb
{
	short nb = 0;
	bool marked = false;
};

std::vector<std::vector<std::string>> boards_str;
std::string numbers_str;
std::vector<short> numbers;
using Board = std::array<std::array<Nb, 5>, 5>;
std::vector<Board> boards;

#if PART == 1
bool part2 = false;
#elif PART == 2
bool part2 = true;
#endif

Board* wanted_board = nullptr;
int final_nb = 0;

void get_input(const std::string& file_path);
void get_numbers();
void get_boards();
void play();

int main()
{
	
	get_input("input.txt");
	get_numbers();
	get_boards();

	play();

	int sum = 0;
	for(auto& l : *wanted_board)
	{
		for(auto& nb : l)
		{
			if(!nb.marked)
				sum += nb.nb;
		}
	}

	std::cout << sum * final_nb << std::endl;

	return 0;
}

void get_input(const std::string& file_path)
{
	boards_str.push_back({});

	std::string line;
	std::ifstream input_file{file_path};

	std::getline(input_file, numbers_str);
	std::getline(input_file, line);

	int index = 0;
	while(std::getline(input_file, line))
	{
		if(line == "")
		{
			index++;
			boards_str.push_back({});
		}
		else
			boards_str[index].push_back(line);
	}
}

void get_numbers()
{
	while(numbers_str.size())
	{
		auto it = numbers_str.find(',');
		if(it != std::string::npos)
		{
			numbers.push_back(std::stoi(std::string(numbers_str.begin(), numbers_str.begin() + it)));
			numbers_str.erase(numbers_str.begin(), numbers_str.begin() + it + 1);
		}
		else
		{
			numbers.push_back(std::stoi(numbers_str));
			numbers_str.erase(numbers_str.begin(), numbers_str.end());
		}
	}
}

void get_boards()
{
	for(size_t index = 0; index < boards_str.size(); index++)
	{
		boards.push_back({});
		for(size_t i1 = 0; i1 < boards_str[index].size(); i1++)
		{
			std::istringstream iss{boards_str[index][i1]};
			short nb;
			for(size_t i2 = 0; i2 < boards[index][i1].size(); i2++)
			{
				iss >> nb;
				boards[index][i1][i2] = {nb, false};
			}
		}
	}
}

void play()
{
	for(auto number : numbers)
	{
		if(wanted_board)
			break;
		for(int index = 0; index < boards.size() && !wanted_board; index++)
		{
			auto& board = boards[index];
			for(size_t i1 = 0; i1 < board.size(); i1++)
			{
				for(size_t i2 = 0; i2 < board[i1].size(); i2++)
				{
					if(number == board[i1][i2].nb)
						board[i1][i2].marked = true;
				}
			}
			for (size_t i1 = 0; i1 < board.size(); i1++)
			{
				if (board[i1][0].marked &&
					board[i1][1].marked &&
					board[i1][2].marked &&
					board[i1][3].marked &&
					board[i1][4].marked)
				{
					if(!part2)
					{
						wanted_board = &board;
						final_nb = number;
						break;
					}
					else
					{
						boards.erase(boards.begin() + index);
						index--;
						if(boards.size() == 1)
							part2 = false;
					}
				}
			}
			for (size_t i2 = 0; i2 < board.size(); i2++)
			{
				if (board[0][i2].marked &&
					board[1][i2].marked &&
					board[2][i2].marked &&
					board[3][i2].marked &&
					board[4][i2].marked)
				{
					if(!part2)
					{
						wanted_board = &board;
						final_nb = number;
						break;
					}
					else
					{
						boards.erase(boards.begin() + index);
						index--;
						if(boards.size() == 1)
							part2 = false;
					}
				}
			}
		}
	}
}