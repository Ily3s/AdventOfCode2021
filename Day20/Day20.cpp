#include <iostream>
#include <string>
#include <fstream>
#include <vector>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 2			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

using ushort = unsigned short;
using ull_int = unsigned long long int;

std::string program;

using Image = std::vector<std::string>;

Image image;
char back_ground = '0';

void get_input();
void resize_image();
void enhance_image();
size_t white_pixels(const Image& img);

std::ostream& operator<<(std::ostream& os, const Image& img);

int main()
{

	get_input();
	
	for (ushort i = 0; i < 2+(PART-1)*48; i++)
	{
		resize_image();
		enhance_image();
	}

	std::cout << white_pixels(image) << std::endl;

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{ "example.txt" };
	#elif EXAMPLE == 0
	std::ifstream input_file{ "input.txt" };
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
	image.insert(image.begin(), std::string(image[0].size(), back_ground)); // top
	image.push_back(std::string(image[0].size(), back_ground)); 			// bot

	for (auto& row : image)													// left
		row.insert(row.begin(), back_ground);
	for (auto& row : image)
		row.push_back(back_ground);											// right
}

void enhance_image()
{

	Image output = image;
	for (int y = 0; y < image.size(); y++)
	{
		auto& row = image[y];
		for (int x = 0; x < row.size(); x++)
		{
			std::string neighbors;

			for (int j = y - 1; j <= y + 1; j++)
			{
				for (int i = x - 1; i <= x + 1; i++)
				{
					if (j < 0 || j >= image.size())
						neighbors.push_back(back_ground);
					else if (i < 0 || i >= image[j].size())
						neighbors.push_back(back_ground);
					else
						neighbors.push_back(image[j][i]);
				}
			}

			output[y][x] = program[std::stoi(neighbors, nullptr, 2)];
		}
	}
	image = output;

	#if EXAMPLE == 0
	back_ground = (back_ground - '0') ? '0' : '1';
	#endif
}

size_t white_pixels(const Image& img)
{
	if (back_ground == '1')
		return SIZE_MAX;
	
	size_t count = 0;

	for (size_t y = 0; y < img.size(); y++)
	{
		auto& row = img[y];
		for (size_t x = 0; x < row.size(); x++)
		{
			if (row[x] == '1')
				count++;
		}
	}

	return count;
}

std::ostream& operator<<(std::ostream& os, const Image& img)
{
	for (size_t y = 0; y < image.size(); y++)
	{
		auto& row = image[y];
		for (size_t x = 0; x < row.size(); x++)
			os << (row[x] == '1' ? '#' : '.');
		os << '\n';
	}
	return os;
}