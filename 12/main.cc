#include "../common/fileparse.hh"
#include <iostream>
// #include <fstream>
// #include <map>
// #include <numeric>
// #include <ranges>
#include <string>
#include <vector>

// void print_map(std:: stones);
using Map = std::vector<std::string>;
using Fences = std::vector<std::vector<int>>;

struct Pos {
	int x; //row
	int y; //col

	// 	constexpr bool operator<=>(Pos const &other) const = default;
	// 	constexpr bool operator<(Pos const &other) const {
	// 		return x < other.x || (x == other.x && y < other.y);
	// 	}
	constexpr Pos operator+(Pos const &other) const {
		return {x + other.x, y + other.y};
	}
};

constexpr bool in_bounds(Pos pos, const Pos MAX) {
	return std::clamp(pos.x, 0, MAX.x - 1) == pos.x && std::clamp(pos.y, 0, MAX.y - 1) == pos.y;
}
constexpr std::array<Pos, 4> Dirs{{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}};

// constexpr std::vector<Pos> inbound_steps(Pos pos, Pos MAX) {
// 	constexpr std::array<Pos, 4> Dirs{{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}};

// 	std::vector<Pos> nexts;
// 	for (auto dir : Dirs) {
// 		Pos next{pos.x + dir.x, pos.y + dir.y};
// 		if (in_bounds(next, MAX))
// 			nexts.push_back(next);
// 	}

// 	return nexts;
// }

Fences calc_fences(Map const &map) {
	Fences fences;

	const Pos MAX(map.size(), map.front().size());

	Pos pos{0, 0};

	for (pos.y = 0; auto const &line : map) {
		fences.push_back({});

		for (pos.x = 0; auto c : line) {
			const char plottype = map[pos.y][pos.x];
			auto numfences = 0;

			for (auto dir : Dirs) {
				auto neighbor = pos + dir;
				if (!in_bounds(neighbor, MAX) || map[neighbor.y][neighbor.x] != plottype) {
					numfences++;
				}
			}

			fences.back().push_back(numfences);
			pos.x++;
		}
		pos.y++;
	}

	return fences;
}

int main(int argc, char *argv[]) {
	auto map = parse_lines(argv[1]);

	auto fences = calc_fences(map);

	for (auto const &row : map) {
		std::cout << row;
		std::cout << "\n";
	}

	for (auto const &row : fences) {
		for (auto const &col : row) {
			std::cout << col << " ";
		}
		std::cout << "\n";
	}

	// std::cout << "Part 1: " << total << "\n";

	// std::cout << "Part 2: " << total2 << "\n";

	return 0;
}
