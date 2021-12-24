#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>

/*
This code is currently WIP
If you uncomment the main func, this will cause a stack buffer overflow
*/

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 1			// define as 1 to output part 1
#define EXAMPLE 1 		// define as 1 to take input in the example

struct Vec2
{
	uint8_t x = 0, y = 0;

	Vec2() = default;
	constexpr Vec2(uint8_t x, uint8_t y) : x(x), y(y) {}

	bool operator==(const Vec2& other) const
	{
		return x == other.x && y == other.y;
	}
};

struct Amphipod
{
	uint8_t type = 0;
	Vec2 position;

	Amphipod(uint8_t t, const Vec2& vec) : type(t), position(vec) {}
	Amphipod() = default;
};

std::array<Amphipod, 8> amphipods;
std::vector<uint32_t> costs;

constexpr std::array<Vec2, 15> possible_dests
{
	Vec2{0, 0},
	Vec2{1, 0},
	Vec2{2, 1},
	Vec2{2, 2},
	Vec2{3, 0},
	Vec2{4, 1},
	Vec2{4, 2},
	Vec2{5, 0},
	Vec2{6, 1},
	Vec2{6, 2},
	Vec2{7, 0},
	Vec2{8, 1},
	Vec2{8, 2},
	Vec2{9, 0},
	Vec2{10, 0}
};

bool is_ordered();
bool is_any_blocking(const Vec2& start, const Vec2& dest);
void find_path();

void get_input();

int main()
{
	/*
	get_input();
	find_path();

	uint32_t min = *std::min_element(costs.begin(), costs.end());

	std::cout << min << std::endl;
	*/

	std::cout << "This code is currently WIP" << std::endl;
	std::cout << "If you uncomment the main func, this will cause a stack buffer overflow" << std::endl;

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{ "example.txt" };
	#elif EXAMPLE == 0
	std::ifstream input_file{ "input.txt" };
	#endif

	std::string line;

	// Ignore the useless lines
	std::getline(input_file, line);
	std::getline(input_file, line);

	uint8_t index = 0;
	for (uint8_t y = 1; std::getline(input_file, line); y++)
	{
		for (int8_t x = 0; x < 10; x++)
		{
			if (line[x] == '#' || line[x] == ' ')
				continue;

			amphipods[index] = { static_cast<uint8_t>(line[x] - 'A'), {static_cast<uint8_t>(x - 1), y} };
			index++;
		}
	}
}

inline bool its_place(const Amphipod& apod, const Vec2& dest)
{
	return 2 + apod.type * 2 != apod.position.x;
}

inline uint32_t move_cost(const Vec2& start, const Vec2& dest)
{
	return std::abs(start.x - dest.x) + start.y + dest.y;
}

void find_path()
{
	static uint32_t cost = 0;

	for (Amphipod& apod : amphipods)
	{
		for (const Vec2& dest : possible_dests)
		{
			if (!dest.y && !apod.position.y)
				continue;

			// If the move is stupid
			if (dest.x == apod.position.x)
				continue;

			if (dest.y)
			{
				if (!its_place(apod, dest))
					continue;

				for (const Amphipod& neighbor : amphipods)
				{
					if (neighbor.position.y == dest.y && neighbor.type != apod.type)
						continue;
				}
			}

			if (is_any_blocking(apod.position, dest))
				continue;

			Vec2 prev_pos = apod.position;
			apod.position = dest;
			uint32_t additional_cost = move_cost(apod.position, dest);
			cost += additional_cost;

			if (is_ordered())
			{
				costs.push_back(cost);
				cost -= additional_cost;
				apod.position = prev_pos;
				continue;
			}

			find_path();
		}
	}
}

bool is_any_blocking(const Vec2& start, const Vec2& dest)
{
	for (const Amphipod& apod : amphipods)
	{
		if (apod.position == start)
			continue;

		if (apod.position == dest)
			return true;

		uint8_t min = std::min(start.x, dest.x);
		uint8_t max = std::max(start.x, dest.x);

		if (!apod.position.y && (apod.position.x >= min && apod.position.x <= max))
			return true;
		else if (apod.position.x == start.x && apod.position.y > start.y)
			return true;
		else if (apod.position.x == dest.x && apod.position.y > dest.y)
			return true;
	}

	return false;
}

bool is_ordered()
{
	for (const Amphipod& apod : amphipods)
	{
		if (!its_place(apod, apod.position))
			return false;

		if (!apod.position.y)
			return false;
	}

	return true;
}