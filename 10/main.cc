#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

struct Pos {
	int x; //row
	int y; //col

	constexpr bool operator<=>(Pos const &other) const = default;
	constexpr bool operator<(Pos const &other) const {
		return x < other.x || (x == other.x && y < other.y);
	}
};

struct ParsedData {
	std::vector<std::vector<int>> topo;
	std::vector<Pos> trailheads;
};

ParsedData parse_packed_digits(std::string_view filename) {
	std::ifstream file{std::string(filename)};

	std::vector<std::vector<int>> topo;
	std::vector<Pos> trailheads;

	// Read each line from the file
	for (std::string line; std::getline(file, line);) {

		// Split the line into integers
		std::vector<int> ints;
		ints.reserve(line.length());

		for (auto c : line) {
			if (int i = c - '0'; i >= 0 && i <= 9) {
				if (i == 0)
					trailheads.push_back(Pos(ints.size(), topo.size()));
				ints.push_back(i);
			}
		}

		topo.push_back(ints);
	}

	return {topo, trailheads};
}

constexpr bool in_bounds(Pos pos, Pos MAX) {
	return std::clamp(pos.x, 0, MAX.x - 1) == pos.x && std::clamp(pos.y, 0, MAX.y - 1) == pos.y;
}

constexpr std::vector<Pos> inbound_steps(Pos pos, Pos MAX) {
	constexpr std::array<Pos, 4> Dirs{{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}};

	std::vector<Pos> nexts;
	for (auto dir : Dirs) {
		Pos next{pos.x + dir.x, pos.y + dir.y};
		if (in_bounds(next, MAX))
			nexts.push_back(next);
	}

	return nexts;
}
static_assert(inbound_steps({0, 0}, {99, 99}) == std::vector<Pos>{{0, 1}, {1, 0}});
static_assert(inbound_steps({1, 0}, {99, 99}) == std::vector<Pos>{{1, 1}, {0, 0}, {2, 0}});

struct TrailData {
	unsigned paths = 0;
	std::set<Pos> summits;
};

constexpr void check_trail(Pos head, std::vector<std::vector<int>> const &topo, TrailData &trail) {
	const Pos MAX(topo.front().size(), topo.size());

	const auto cur = head;
	const auto level = topo[cur.y][cur.x];

	if (level == 9) {
		trail.summits.insert(cur);
		trail.paths++;
		return;
	}

	auto nexts = inbound_steps(cur, MAX);
	for (auto next : nexts) {
		if (topo[next.y][next.x] == level + 1) {
			check_trail(next, topo, trail);
		}
	}
}

int main(int argc, char *argv[]) {
	const auto [topo, trailheads] = parse_packed_digits(argv[1]);

	unsigned unique = 0;
	unsigned total = 0;

	for (auto head : trailheads) {
		TrailData trail;
		check_trail(head, topo, trail);
		unique += trail.summits.size();
		total += trail.paths;
	}

	std::cout << "Part 1: " << unique << "\n";
	//709
	std::cout << "Part 2: " << total << "\n";
	//1326

	return 0;
}
