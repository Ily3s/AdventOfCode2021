#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include <unordered_set>

/*
Defining macros to be able to switch from example to input
and from part 1 to part 2 easily
*/
#define PART 1			// define as 1 to output part 1
#define EXAMPLE 0 		// define as 1 to take input in the example

using ushort = unsigned short;
using ull_int = unsigned long long int;

struct Vec3
{
	int x = 0, y = 0, z = 0;
	Vec3(int x, int y, int z) : x(x), y(y), z(z) {}
	Vec3() = default;

	bool operator==(const Vec3 other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	Vec3 operator+(const Vec3& p) const
	{
		return Vec3(x + p.x, y + p.y, z + p.z);
	}

	Vec3 operator-(const Vec3& p) const
	{
		return Vec3(x - p.x, y - p.y, z - p.z);
	}
};

namespace std
{
	template <>
	struct hash<Vec3>
	{
		size_t operator()(const Vec3& vec) const
		{
			return vec.z * 2000 * 2000 + vec.y * 2000 + vec.x;
		}
	};
}

struct Scanner
{
	Vec3 pos;
	std::vector<Vec3> beacons;
	std::array<std::vector<Vec3>, 24> b2b_vecs;
	int orientation = -1;

	Scanner() = default;
};

std::vector<Scanner> scanners;
std::unordered_set<Vec3> beacons;

std::array<std::function<Vec3(const Vec3&)>, 24> transforms
{
	[](const Vec3& v) { return Vec3{ v.x,  v.y,  v.z}; },
	[](const Vec3& v) { return Vec3{ v.x, -v.z,  v.y}; },
	[](const Vec3& v) { return Vec3{ v.x, -v.y, -v.z}; },
	[](const Vec3& v) { return Vec3{ v.x,  v.z, -v.y}; },
	[](const Vec3& v) { return Vec3{-v.y,  v.x,  v.z}; },
	[](const Vec3& v) { return Vec3{ v.z,  v.x,  v.y}; },
	[](const Vec3& v) { return Vec3{ v.y,  v.x, -v.z}; },
	[](const Vec3& v) { return Vec3{-v.z,  v.x, -v.y}; },
	[](const Vec3& v) { return Vec3{-v.x, -v.y,  v.z}; },
	[](const Vec3& v) { return Vec3{-v.x, -v.z, -v.y}; },
	[](const Vec3& v) { return Vec3{-v.x,  v.y, -v.z}; },
	[](const Vec3& v) { return Vec3{-v.x,  v.z,  v.y}; },
	[](const Vec3& v) { return Vec3{ v.y, -v.x,  v.z}; },
	[](const Vec3& v) { return Vec3{ v.z, -v.x, -v.y}; },
	[](const Vec3& v) { return Vec3{-v.y, -v.x, -v.z}; },
	[](const Vec3& v) { return Vec3{-v.z, -v.x,  v.y}; },
	[](const Vec3& v) { return Vec3{-v.z,  v.y,  v.x}; },
	[](const Vec3& v) { return Vec3{ v.y,  v.z,  v.x}; },
	[](const Vec3& v) { return Vec3{ v.z, -v.y,  v.x}; },
	[](const Vec3& v) { return Vec3{-v.y, -v.z,  v.x}; },
	[](const Vec3& v) { return Vec3{-v.z, -v.y, -v.x}; },
	[](const Vec3& v) { return Vec3{-v.y,  v.z, -v.x}; },
	[](const Vec3& v) { return Vec3{ v.z,  v.y, -v.x}; },
	[](const Vec3& v) { return Vec3{ v.y, -v.z, -v.x}; }
};

void get_input();
void compute_b2b_vecs();
Vec3 reverse_transform(const Scanner& scan, ushort vec_index);
void make_scanners_abs();
void build_beacons();

int main()
{

	get_input();
	compute_b2b_vecs();
	make_scanners_abs();
	build_beacons();

	std::cout << beacons.size() << std::endl;

}

void get_input()
{
	#if EXAMPLE == 1
	std::ifstream input_file{ "example.txt" };
	#elif EXAMPLE == 0
	std::ifstream input_file{ "input.txt" };
	#endif

	std::string line;

	int n = -1;

	while (std::getline(input_file, line))
	{
		if (line[0] == '-' && line[1] == '-')
		{
			scanners.push_back({});
			n++;
		}
		else if (line.size() > 1)
		{

			int x = std::stoi(std::string{ line.begin(), line.begin() + line.find(',') });
			line.erase(line.begin(), line.begin() + line.find(',') + 1);
			int y = std::stoi(std::string{ line.begin(), line.begin() + line.find(',') });
			line.erase(line.begin(), line.begin() + line.find(',') + 1);
			int z = std::stoi(std::string{ line.begin(), line.end() });

			scanners[n].beacons.emplace_back(x, y, z);
		}
	}
}

void compute_b2b_vecs()
{
	for (Scanner& scan : scanners)
	{
		for (int i = 0; i < scan.beacons.size(); i++)
		{
			for (int j = i + 1; j < scan.beacons.size(); j++)
			{
				Vec3 vec_between_beacons
				{
					scan.beacons[i].x - scan.beacons[j].x,
					scan.beacons[i].y - scan.beacons[j].y,
					scan.beacons[i].z - scan.beacons[j].z
				};
				scan.b2b_vecs[0].push_back(vec_between_beacons);
				for (ushort orient = 1; orient < 24; orient++)
					scan.b2b_vecs[orient].push_back(transforms[orient](scan.b2b_vecs[0].back()));
			}
		}
	}
}

Vec3 reverse_transform(const Scanner& scan, ushort vec_index)
{
	const Vec3& untransformed = scan.b2b_vecs[0][vec_index];
	for (int i = 0; i < scan.beacons.size(); i++)
	{
		for (int j = i + 1; j < scan.beacons.size(); j++)
		{
			Vec3 vec_between_beacons
			{
				scan.beacons[i].x - scan.beacons[j].x,
				scan.beacons[i].y - scan.beacons[j].y,
				scan.beacons[i].z - scan.beacons[j].z
			};
			if (vec_between_beacons == untransformed)
				return transforms[scan.orientation](scan.beacons[i]);
		}
	}
	return { -1, -1, -1 };
}

void make_scanners_abs()
{
	scanners[0].orientation = 0;
	std::vector<Scanner*> abs_scanners{ &scanners[0] };
	abs_scanners.reserve(scanners.size());

	for (short i1 = 0; i1 < scanners.size(); i1++)
	{
		Scanner& scan1 = *abs_scanners[i1];

		for (ushort i2 = 0; i2 < scanners.size(); i2++)
		{
			Scanner& scan2 = scanners[i2];

			if (i1 == i2)
				continue;

			if (std::find(abs_scanners.begin(), abs_scanners.end(), &scan2) != abs_scanners.end())
				continue;

			bool found_orientation = false;

			for (ushort orient = 0; orient < 24; orient++)
			{
				ushort match_nb = 0;

				for (ushort index_vec = 0; index_vec < scan1.b2b_vecs[scan1.orientation].size(); index_vec++)
				{
					const Vec3& beac_vec1 = scan1.b2b_vecs[scan1.orientation][index_vec];
					ushort index2b = 0;
					ushort index2 = 0;
					for (const Vec3& beac_vec2 : scan2.b2b_vecs[orient])
					{
						if (beac_vec1 == beac_vec2)
						{
							index2b = index2;
							match_nb++;
						}
						index2++;
					}
					if (match_nb >= 12)
					{
						found_orientation = true;
						scan2.orientation = orient;
						abs_scanners.push_back(&scan2);
						i1--;

						Vec3 transformed_beacon1 = reverse_transform(scan1, index_vec);
						Vec3 transformed_beacon2 = reverse_transform(scan2, index2b);
						scan2.pos = scan1.pos + transformed_beacon1 - transformed_beacon2;

						break;
					}
				}

				if (found_orientation)
					break;
			}
		}
	}

}

void build_beacons()
{
	for (const Scanner& scan : scanners)
	{
		for (const Vec3& beacon : scan.beacons)
		{
			const Vec3 beacon_abs = transforms[scan.orientation](beacon) + scan.pos;
			beacons.insert(beacon_abs);
		}
	}
}