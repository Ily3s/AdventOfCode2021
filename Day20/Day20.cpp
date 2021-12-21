#include <iostream>
#include <string>
#include <fstream>
#include <vector>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 1 		// define as 1 to take input in the example

using ushort = unsigned short;
using ull_int = unsigned long long int;

std::string program;

using Image = std::vector<std::vector<char>>;

Image image;

void get_input();
void resize_image();
void enhance_image();
size_t white_pixels();

int main()
{

	get_input();

	for (ushort i = 0; i < 2; i++)
	{
		resize_image();
		enhance_image();
	}

	std::cout << white_pixels() << std::endl;

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{"example.txt"};
	#elif EXAMPLE == 0
	std::ifstream input_file{"input.txt"};
	#endif

	std::string line;

	// geting the program
	std::getline(input_file, line);
	for (char c : line)
		program.push_back(c == '#' ? '1' : '0');
	std::getline(input_file, line);

	// geting the input image
	for (ushort i = 0; std::getline(input_file, line); i++)
	{
		image.push_back({});
		for (char c : line)
			image[i].push_back(c == '#' ? '1' : '0');
	}
}

void resize_image()
{
	// if a pixel is white on an edge, this edge need to be resized

	bool need_resize_top = false;
	bool need_resize_bot = false;
	bool need_resize_left = false;
	bool need_resize_right = false;

	for (size_t x = 0; x < image.size(); x++)
	{
		auto& column = image[x];
		for (size_t y = 0; y < column.size(); y++)
		{
			char pix = column[y];

			if (pix == '1')
			{
				if (x == 0)
					need_resize_left = true;
				if (x == image.size() - 1)
					need_resize_right = true;
				if (y == 0)
					need_resize_top = true;
				if (y == column.size() - 1)
					need_resize_bot = true;
			}
		}
	}

	if (need_resize_left)
	{
		image.push_back(std::vector(image[0].size(), '0'));
		for (size_t x = image.size() - 1; x > 0; x--)
			image[x] = image[x-1];
	}
	if (need_resize_right)
	{
		image.push_back(std::vector(image[0].size(), '0'));
	}
	if (need_resize_top)
	{
		for (auto& column : image)
		{
			column.push_back('0');
			for (size_t y = column.size() - 1; y > 0; y--)
				column[y] = column[y-1];
		}
	}
	if (need_resize_bot)
	{
		for (auto& column : image)
			column.push_back('0');
	}
}

void enhance_image()
{
	static char back_ground = '0';

	Image output = image;
	for (int x = 0; x < image.size(); x++)
	{
		auto& column = image[x];
		for (int y = 0; y < column.size(); y++)
		{
			std::string neighbors;

			for (int j = y - 1; j <= y + 1; j++)
			{
				for (int i = x - 1; i <= x + 1; i++)
				{
					if (j < 0 || j >= image.size())
						neighbors.push_back(back_ground);
					else if (i < 0 || i >= image.size())
						neighbors.push_back(back_ground);
					else
						neighbors.push_back(image[i][j]);
				}
			}

			output[x][y] = program[std::stoi(neighbors, nullptr, 2)];
		}
	}
	image = output;

	back_ground = (back_ground - '0') ? '0' : '1';
}

size_t white_pixels()
{
	size_t count = 0;

	for (size_t x = 0; x < image.size(); x++)
	{
		auto& column = image[x];
		for (size_t y = 0; y < column.size(); y++)
		{
			if (column[y] == '1')
				count++;
		}
	}

	return count;
}