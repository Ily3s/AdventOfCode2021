#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#define EXAMPLE 0 		// define as 1 to take input in the example

struct Vec2
{
	int x = 0, y = 0;
};

Vec2 target_min, target_max;

int max_y_pos = 0;

void get_input();

bool is_solution(Vec2 velocity);
int inverse_triangular_distribution(int x);

size_t find_solutions();

int main()
{

	get_input();

	size_t solutions_count =  find_solutions();

	std::cout << "Part 1 : " << max_y_pos << '\n';
	std::cout << "Part 2 : " << solutions_count << '\n';

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

	std::string x_min_str{line.begin() + 15, line.begin() + line.find('.')};
	line.erase(line.begin(), line.begin() + line.find('.'));
	std::string x_max_str{line.begin() + 2, line.begin() + line.find(',')};
	line.erase(line.begin(), line.begin() + line.find(','));
	std::string y_min_str{line.begin() + 4, line.begin() + line.find('.')};
	line.erase(line.begin(), line.begin() + line.find('.'));
	std::string y_max_str{line.begin() + 2, line.end()};

	target_min.x = std::stoi(x_min_str);
	target_min.y = std::stoi(y_min_str);
	target_max.x = std::stoi(x_max_str);
	target_max.y = std::stoi(y_max_str);
}

bool is_solution(Vec2 velocity)
{
	Vec2 position{0, 0};
	int possible_max_y_pos = 0;
	while(position.x <= target_max.x && position.y >= target_min.y)
	{
		if(position.x >= target_min.x && position.y <= target_max.y)
		{
			if (possible_max_y_pos > max_y_pos) max_y_pos = possible_max_y_pos;
			return true;
		}

		position.x += velocity.x;
		position.y += velocity.y;

		if (position.y > possible_max_y_pos) possible_max_y_pos = position.y;

		velocity.y--;
		if (--velocity.x < 0) velocity.x = 0;
	}
	return false;
}

/* If you have no idea what's the purpose of this function
Research it's name */
int inverse_triangular_distribution(int x)
{
	return static_cast<int>(std::sqrt(2 * x));
}

size_t find_solutions()
{
	Vec2 velocity{inverse_triangular_distribution(target_min.x), target_min.y};
	size_t solutions_count = 0;

	for(; velocity.x <= target_max.x; velocity.x++)
		for(velocity.y = target_min.y; velocity.y < - target_min.y; velocity.y++)
			if (is_solution(velocity)) solutions_count++;

	return solutions_count;
}