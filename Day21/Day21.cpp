#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <unordered_map>
#include <algorithm>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 1 		// define as 1 to take input in the example

struct Player
{
	uint8_t pos = 0;
	uint16_t score = 0;

	std::array<std::array<uint64_t, 10>, 22> multiverse;

	uint64_t winning_universes = 0;
};

uint8_t dice = 0;
uint16_t rolls = 0;

Player p1, p2;

void get_input();
void play_turn();

void play_multiverse();
bool is_game_ended();

uint64_t temp = 0;

std::unordered_map<uint8_t, uint8_t> possibilities
{
	{3, 1},
	{4, 3},
	{5, 6},
	{6, 7},
	{7, 6},
	{8, 3},
	{9, 1}
};

int main()
{

	get_input();

	#if PART == 1

	while (p1.score < 1000 && p2.score < 1000)
		play_turn();

	uint16_t min_score = std::min(p1.score, p2.score);

	std::cout << (uint32_t)min_score * rolls << std::endl;

	#elif PART == 2

	while (!is_game_ended())
		play_multiverse();

	uint64_t max_score = std::max(p1.winning_universes, p2.winning_universes);

	std::cout << max_score << std::endl;

	#endif

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{ "example.txt" };
	#elif EXAMPLE == 0
	std::ifstream input_file{ "input.txt" };
	#endif

	std::string line;

	std::getline(input_file, line);
	p1.pos = line[28] - '0';
	if (line.size() > 29)
		p1.pos = p1.pos * 10 + (line[29] - '0');

	p1.multiverse[0][p1.pos - 1] = 1;

	std::getline(input_file, line);
	p2.pos = line[28] - '0';
	if (line.size() > 29)
		p2.pos = p2.pos * 10 + (line[29] - '0');

	p2.multiverse[0][p2.pos - 1] = 1;
}

uint8_t get_dice()
{
	rolls++;
	dice = (dice + 1) % 100;
	if (dice == 0)
		dice = 100;
	return dice;
}

void play_turn()
{
	p1.pos = (p1.pos + get_dice() + get_dice() + get_dice()) % 10;
	if (p1.pos == 0)
		p1.pos = 10;
	p1.score += p1.pos;

	if (p1.score >= 1000)
		return;

	p2.pos = (p2.pos + get_dice() + get_dice() + get_dice()) % 10;
	if (p2.pos == 0)
		p2.pos = 10;
	p2.score += p2.pos;
}

void play_multiverse()
{
	uint64_t winning_p1 = 0, winning_p2 = 0;

	auto temp_p1_multi = p1.multiverse;
	auto temp_p2_multi = p2.multiverse;

	for (uint8_t score = 0; score < 21; score++)
	{
		for (uint8_t pos = 0; pos < 10; pos++)
		{
			for (auto& poss : possibilities)
			{
				uint8_t new_pos = (pos + poss.first) % 10;
				uint8_t new_score = (new_pos + 1 + score) > 21 ? 21 : new_pos + 1 + score;
				temp_p1_multi[new_score][new_pos] += p1.multiverse[score][pos] * poss.second;
				temp_p2_multi[new_score][new_pos] += p2.multiverse[score][pos] * poss.second;

				if (new_score == 21)
				{
					winning_p1 += p1.multiverse[new_score][new_pos] * poss.second;
					winning_p2 += p2.multiverse[new_score][new_pos] * poss.second;
				}
			}

			temp_p1_multi[score][pos] -= p1.multiverse[score][pos];
			temp_p2_multi[score][pos] -= p2.multiverse[score][pos];
		}
	}

	p1.multiverse = temp_p1_multi;
	p2.multiverse = temp_p2_multi;

	p2.winning_universes += winning_p2;
	p1.winning_universes += winning_p1;
}

bool is_game_ended()
{
	bool p1_ended = true, p2_ended = true;
	for (uint8_t score = 0; score < 21; score++)
	{
		auto& sc = p1.multiverse[score];
		if (std::find_if(sc.begin(), sc.end(), [](uint64_t& u) {return u; }) != sc.end())
			p1_ended = false;

		sc = p2.multiverse[score];
		if (std::find_if(sc.begin(), sc.end(), [](uint64_t& u) {return u; }) != sc.end())
			p2_ended = false;
	}
	return p1_ended || p2_ended;
}