#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define EXAMPLE 0 		// define as 1 to take input in the example

enum class State
{
	Empty,
	FacingEast,
	FacingSouth
};

std::vector<std::vector<State>> sea_floor;

void get_input();
bool play_step(State state);

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<State>>& sea_map);

int main()
{

	get_input();

	uint32_t step_count = 0;
	//std::cout << sea_floor << std::endl;
	for (bool b1 = true, b2 = true; b1 || b2;)
	{
		step_count++;
		//std::cout << sea_floor << std::endl;
		b1 = play_step(State::FacingEast);
		b2 = play_step(State::FacingSouth);
	}

	std::cout << step_count << std::endl;

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{ "example.txt" };
	#elif EXAMPLE == 0
	std::ifstream input_file{ "input.txt" };
	#endif

	std::string line;

	while (std::getline(input_file, line))
	{
		sea_floor.push_back({});

		for (char c : line)
			sea_floor.back().push_back(c == '.' ? State::Empty : c == '>' ? State::FacingEast : State::FacingSouth);
	}
}

bool play_step(State state)
{
	std::vector<std::vector<State>> prev_state = sea_floor;
	bool has_changed = false;

	for (uint8_t y = 0; y < prev_state.size(); y++)
	{
		auto& row = prev_state[y];
		auto& new_row = sea_floor[y];
		for (uint8_t x = 0; x < row.size(); x++)
		{
			if (row[x] == state)
			{
				uint8_t next_index;
				if (state == State::FacingEast)
					next_index = x == row.size() - 1 ? 0 : x + 1;
				else
					next_index = y == prev_state.size() - 1 ? 0 : y + 1;
				State& next_origin = state == State::FacingEast ? row[next_index] : prev_state[next_index][x];
				State& next_dest = state == State::FacingEast ? new_row[next_index] : sea_floor[next_index][x];
				if (next_origin == State::Empty)
				{
					new_row[x] = State::Empty;
					next_dest = state;
					has_changed = true;
				}
			}
		}
	}

	return has_changed;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<State>>& sea_map)
{
	for (const auto& row : sea_map)
	{
		for (State st : row)
			os << (st == State::Empty ? '.' : st == State::FacingEast ? '>' : 'v');
		os << '\n';
	}
	return os;
}