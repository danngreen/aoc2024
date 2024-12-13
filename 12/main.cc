#include "../common/fileparse.hh"
#include <iostream>
#include <numeric>
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

unsigned count_fences(Map const &map, Pos pos) {
	const Pos MAX(map.size(), map.front().size());
	const char plottype = map[pos.y][pos.x];
	auto numfences = 0;

	for (auto dir : Dirs) {
		auto neighbor = pos + dir;
		if (!in_bounds(neighbor, MAX) || map[neighbor.y][neighbor.x] != plottype) {
			numfences++;
		}
	}
	return numfences;
}

unsigned count_corners(Map const &map, Pos pos) {
	const Pos MAX(map.size(), map.front().size());

	const char plottype = map[pos.y][pos.x];

	auto numfences = 0;

	// TBlr = 2
	// TL = +1
	// TR = +1
	// TLR =
	for (auto dir : Dirs) {
		auto neighbor = pos + dir;
		if (!in_bounds(neighbor, MAX) || map[neighbor.y][neighbor.x] != plottype) {
			numfences++;
		}
	}
	return numfences;
}

struct Score {
	unsigned fences = 0;
	unsigned plots = 0;
	unsigned sides = 0;
};

Score visit_contiguous(Map const &map, Map &visited, Pos pos, char region, Score score = Score{}) {
	const Pos MAX(map.size(), map.front().size());

	if (in_bounds(pos, MAX) && map[pos.y][pos.x] == region && visited[pos.y][pos.x] <= 'Z') {

		std::cout << "Visiting " << pos.x << "," << pos.y << " " << region << " ($" << score.sides << ")\n";

		visited[pos.y][pos.x] = region - 'A' + 'a'; //to_lower

		auto fences = count_fences(map, pos);
		score.fences += fences;
		score.plots++;
		score.sides += fences == 2 ? 1 : fences > 2 ? fences : 0;

		for (auto dir : Dirs) {
			score = visit_contiguous(map, visited, pos + dir, region, score);
		}
	}

	return score;
}

//sides = corners
constexpr Score visit_contiguous_part1(Map const &map, Map &visited, Pos pos, Score score) {
	const Pos MAX(map.size(), map.front().size());

	const char region = map[pos.y][pos.x];

	score.plots++;
	visited[pos.y][pos.x] = region - 'A' + 'a'; //to_lower

	for (auto dir : Dirs) {
		auto neighbor = pos + dir;

		if (in_bounds(neighbor, MAX) && map[neighbor.y][neighbor.x] == region) {
			if (visited[neighbor.y][neighbor.x] <= 'Z') {
				score = visit_contiguous_part1(map, visited, neighbor, score);
			}
		} else {
			score.fences++;
		}
	}

	std::cout << "Visited " << pos.x << "," << pos.y << " " << region << " ($" << score.sides << ")\n";
	return score;
}

struct Region {
	char code;
	Score score;
};

Pos find_next_pos(Map const &visited, Pos pos) {
	const Pos MAX(visited.size(), visited.front().size());

	do {
		if (pos.x < MAX.x - 1) {
			pos.x++;
		} else {
			pos.x = 0;
			pos.y = (pos.y + 1) % MAX.y;
		}

		if (pos.x == 0 && pos.y == 0)
			break;

	} while (visited[pos.y][pos.x] > 'Z');

	return pos;
}

constexpr std::vector<Region> costs(Map const &map) {
	auto visited = map;
	std::vector<Region> regions;

	Pos pos = {0, 0};

	while (true) {
		Score score = visit_contiguous(map, visited, pos, map[pos.y][pos.x], Score{});

		if (score.plots == 0)
			break;

		regions.emplace_back(map[pos.y][pos.x], score);

		pos = find_next_pos(visited, pos);
		if (pos.x == 0 && pos.y == 0)
			break;
		std::cout << "Searching " << map[pos.y][pos.x] << " from " << pos.x << "," << pos.y << "\n";
	}

	return regions;
}

int main(int argc, char *argv[]) {
	auto map = parse_lines(argv[1]);

	auto rc = costs(map);

	for (auto r : rc) {
		std::cout << r.code << ": " << r.score.plots << " x " << r.score.fences << " | " << r.score.sides << "\n";
	}

	auto total = std::accumulate(
		rc.begin(), rc.end(), 0L, [](long sum, Region const &r) { return sum + r.score.fences * r.score.plots; });

	std::cout << "Part 1: " << total << "\n";
	// 1477762

	auto total2 = std::accumulate(
		rc.begin(), rc.end(), 0L, [](long sum, Region const &r) { return sum + r.score.fences * r.score.sides; });

	std::cout << "Part 2: " << total2 << "\n";

	// < 1063288

	return 0;
}
