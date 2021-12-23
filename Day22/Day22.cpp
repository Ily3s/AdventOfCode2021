#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <algorithm>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 1			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

// PART 1 done
// PART 2 WIP

template<typename T>
struct Vec3
{
	T x = 0, y = 0, z = 0;
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	Vec3() = default;

	bool operator==(const Vec3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
};

namespace std
{
	template <>
	struct hash<Vec3<int8_t>>
	{
		size_t operator()(const Vec3<int8_t>& vec) const
		{
			return (vec.x + 50) * 101 * 101 + (vec.y + 50) * 101 + vec.z + 50;
		}
	};
}

struct Zone
{
	Vec3<int32_t> min, max;
	bool on = false;

	void split();
};

using Instruction = Zone;

std::unordered_map<Vec3<int8_t>, bool> cubes;
std::vector<Zone> zones;
std::vector<Instruction> instructions;

void get_input();
void apply_instructions();
uint32_t count_on_cubes();

int main()
{

	get_input();
	apply_instructions();

	std::cout << count_on_cubes() << std::endl;

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{ "example.txt" };
	instructions.reserve(22);
	#elif EXAMPLE == 0
	std::ifstream input_file{ "input.txt" };
	instructions.reserve(420);
	#endif

	std::string line;

	while (std::getline(input_file, line))
	{
		Instruction instru;

		if (line[1] == 'n')
			instru.on = true;

		std::string current_var;

		#define GET_VAR_MIN \
		line.erase(line.begin(), line.begin() + line.find('=') + 1);\
		current_var = {line.begin(), line.begin() + line.find('.')}

		#define GET_VAR_MAX \
		line.erase(line.begin(), line.begin() + line.find('.') + 2);\
		current_var = {line.begin(), line.begin() + line.find(',')}

		GET_VAR_MIN;
		instru.min.x = std::stoi(current_var);
		GET_VAR_MAX;
		instru.max.x = std::stoi(current_var);
		GET_VAR_MIN;
		instru.min.y = std::stoi(current_var);
		GET_VAR_MAX;
		instru.max.y = std::stoi(current_var);
		GET_VAR_MIN;
		instru.min.z = std::stoi(current_var);
		line.erase(line.begin(), line.begin() + line.find('.') + 2);
		current_var = { line.begin(), line.end() };
		instru.max.z = std::stoi(current_var);
		instructions.push_back(instru);
	}
}

void apply_instructions()
{
	cubes.reserve(101 * 101 * 101);
	for (const Instruction& instru : instructions)
	{
		for (int32_t x = instru.min.x; x <= instru.max.x; x++)
		{
			if (x < -50 || x > 50)
				continue;

			for (int32_t y = instru.min.y; y <= instru.max.y; y++)
			{
				if (y < -50 || y > 50)
					continue;

				for (int32_t z = instru.min.z; z <= instru.max.z; z++)
				{
					if (z < -50 || z > 50)
						continue;

					cubes.insert_or_assign(Vec3<int8_t>(x, y, z), instru.on);
				}
			}
		}
	}
}

struct Range
{
	int32_t min, max;
};

void Zone::split()
{

	for (Zone& other : zones)
	{
		std::array<int32_t, 4> xdim{ other.min.x, other.max.x, min.x, max.x };
		std::sort(xdim.begin(), xdim.end());

		if (xdim[0] == other.min.x && xdim[1] == other.max.x)
			continue;
		if (xdim[0] == min.x && xdim[1] == max.x)
			continue;

		Range x_split = { xdim[1], xdim[2] };

		std::array<int32_t, 4> ydim{ other.min.y, other.max.y, min.y, max.y };
		std::sort(ydim.begin(), ydim.end());

		if (ydim[0] == other.min.y && ydim[1] == other.max.y)
			continue;
		if (ydim[0] == min.y && ydim[1] == max.y)
			continue;

		Range y_split = { ydim[1], ydim[2] };

		std::array<int32_t, 4> zdim{ other.min.z, other.max.z, min.z, max.z };
		std::sort(zdim.begin(), zdim.end());

		if (zdim[0] == other.min.z && zdim[1] == other.max.z)
			continue;
		if (zdim[0] == min.z && zdim[1] == max.z)
			continue;

		Range z_split = { zdim[1], zdim[2] };

		// There is a collition


	}

}

uint32_t count_on_cubes()
{
	uint32_t count = 0;

	for (const auto& pair : cubes)
	{
		if (pair.second)
			count++;
	}

	return count;
}