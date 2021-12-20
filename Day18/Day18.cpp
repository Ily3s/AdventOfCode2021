#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

using ushort = unsigned short;

class SnailNumber
{
private:

	std::string m_number;

	void m_explode(size_t index);
	void m_split(size_t index);
	std::string::iterator inline first_not_digit(size_t index);
	std::string::iterator inline first_digit(size_t index);
	size_t get_first_split();
	void get_explosions();
	void update_actions_indexes(size_t index, short offset);

	std::vector<size_t> pending_explosions;

public:

	SnailNumber(const std::string& str) : m_number(str) {}
	SnailNumber(const SnailNumber& other) : m_number(other.m_number) {}

	SnailNumber& operator+=(const SnailNumber& other);
	SnailNumber operator+(const SnailNumber& other) const;

	unsigned int magnitude();
};

enum class Action
{
	Explode,
	Split
};

std::vector<SnailNumber> addition_list;
unsigned int higher_magnitude = 0;

void get_input();

int main()
{

	get_input();

	#if PART == 1

	while (addition_list.size() > 1)
	{
		addition_list.front() += addition_list[1];
		addition_list.erase(addition_list.begin() + 1);
	}

	std::cout << addition_list[0].magnitude() << std::endl;

	#elif PART == 2

	for (ushort index1 = 0; index1 < addition_list.size(); index1++)
	{
		for (ushort index2 = 0; index2 < addition_list.size(); index2++)
		{
			if (index1 == index2)
				continue;

			SnailNumber sum1 = addition_list[index1] + addition_list[index2];
			SnailNumber sum2 = addition_list[index2] + addition_list[index1];

			unsigned int magnitude1 = sum1.magnitude();
			unsigned int magnitude2 = sum2.magnitude();

			higher_magnitude = std::max(higher_magnitude, std::max(magnitude1, magnitude2));
		}
	}

	std::cout << higher_magnitude << std::endl;

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

	while (std::getline(input_file, line))
		addition_list.emplace_back(line);
}

SnailNumber SnailNumber::operator+(const SnailNumber& other) const
{
	SnailNumber output{ *this };
	output += other;
	return output;
}

SnailNumber& SnailNumber::operator+=(const SnailNumber& other)
{
	m_number.insert(0, "[");
	m_number.insert(m_number.size(), ',' + other.m_number + ']');

	get_explosions();

	while (true)
	{
		if (pending_explosions.size())
		{
			m_explode(pending_explosions.front());
			pending_explosions.erase(pending_explosions.begin());
		}
		else
		{
			size_t first_split = get_first_split();
			if (first_split)
				m_split(get_first_split());
			else
				break;
		}
	}

	return *this;
}

unsigned int SnailNumber::magnitude()
{
	while (m_number[0] == '[')
	{
		for (size_t i = 0; i < m_number.size(); i++)
		{
			if (m_number[i] == ',' && m_number[i - 1] != ']' && m_number[i + 1] != '[')
			{
				std::string::iterator end_it = first_not_digit(i + 1);
				std::string::iterator beg_it = m_number.begin() + i;
				for (; *beg_it != '['; beg_it--);

				std::string lhs_str{ beg_it + 1, m_number.begin() + i };
				std::string rhs_str{ m_number.begin() + i + 1, end_it };
				ushort lhs = std::stoi(lhs_str);
				ushort rhs = std::stoi(rhs_str);

				std::string new_value = std::to_string(lhs * 3 + rhs * 2);

				m_number.erase(beg_it, end_it + 1);
				m_number.insert(beg_it - m_number.begin(), new_value);

				i -= lhs_str.size() + rhs_str.size() + 3 - new_value.size();
			}
		}
	}
	return std::stoi(m_number);
}

void SnailNumber::m_split(size_t index)
{
	std::string to_split_str{ m_number[index], m_number[index + 1] };
	m_number.erase(m_number.begin() + index, m_number.begin() + index + 2);

	ushort to_split = std::stoi(to_split_str);
	ushort lhs = (ushort)(to_split / 2);
	ushort rhs = (ushort)((float)to_split / 2 + 0.5f);

	std::string new_pair = { '[' + std::to_string(lhs) + ',' + std::to_string(rhs) + ']' };
	m_number.insert(index, new_pair);

	get_explosions();
}

void SnailNumber::m_explode(size_t index)
{
	auto comma_it = first_not_digit(index + 1);
	auto end_it = first_not_digit(comma_it - m_number.begin() + 1);
	std::string lhs_str{ m_number.begin() + index + 1, comma_it };
	std::string rhs_str{ comma_it + 1, end_it };

	m_number.erase(m_number.begin() + index, end_it + 1);
	m_number.insert(index, "0");

	update_actions_indexes(index, -(int)(lhs_str.size() + rhs_str.size() + 2));

	ushort lhs = std::stoi(lhs_str);
	ushort rhs = std::stoi(rhs_str);

	auto it = first_digit(index + 2);
	if (it != m_number.end())
	{
		std::string old_value{ it, first_not_digit(it - m_number.begin()) };
		std::string new_value = std::to_string(std::stoi(old_value) + rhs);
		if (old_value.size() == 1)
			m_number.erase(it);
		else
			m_number.erase(it, it + 2);
		m_number.insert(it - m_number.begin(), new_value);
		if (new_value.size() == 2 && old_value.size() != 2)
			update_actions_indexes(it - m_number.begin(), 1);

	}

	for (std::string::iterator it = m_number.begin() + index - 1;
		it > m_number.begin(); it--)
	{
		if (std::isdigit(*it))
		{
			std::string old_value{ *it };
			if (std::isdigit(*(it - 1)))
				old_value.insert(old_value.begin(), *(it - 1));
			std::string new_value = std::to_string(std::stoi(old_value) + lhs);
			if (old_value.size() == 1)
				m_number.erase(it);
			else
			{
				m_number.erase(it - 1, it + 1);
				it--;
			}
			m_number.insert(it - m_number.begin(), new_value);

			if (new_value.size() == 2 && old_value.size() != 2)
				update_actions_indexes(it - m_number.begin(), 1);

			break;
		}
	}

}

void SnailNumber::get_explosions()
{
	ushort brackets = 0;
	for (size_t index = 0; index < m_number.size(); index++)
	{
		const char& c = m_number[index];
		if (c == '[') brackets++;
		if (c == ']') brackets--;

		if (c == '[' && brackets > 4)
			pending_explosions.push_back(index);
	}
}

size_t SnailNumber::get_first_split()
{
	for (size_t index = 1; index < m_number.size(); index++)
	{
		if (std::isdigit(m_number[index - 1]) && std::isdigit(m_number[index]))
			return index - 1;
	}
	return 0;
}

std::string::iterator SnailNumber::first_not_digit(size_t index)
{
	return std::find_if_not(m_number.begin() + index, m_number.end(), [](char c) {return std::isdigit(c); });
}

std::string::iterator SnailNumber::first_digit(size_t index)
{
	return std::find_if(m_number.begin() + index, m_number.end(), [](char c) {return std::isdigit(c); });
}

void SnailNumber::update_actions_indexes(size_t index, short offset)
{
	for (size_t& action_index : pending_explosions)
	{
		if (action_index >= index)
			action_index += offset;
	}
}