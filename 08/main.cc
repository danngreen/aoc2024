#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <span>
#include <vector>

struct Pos {
	int x;
	int y;
	auto operator<=>(Pos const &other) const = default;
};

Pos MAX;

void print_grid(std::set<Pos> antinodes, std::map<char, std::vector<Pos>> map);

bool in_bounds(Pos a) {
	return a.x >= 0 && a.x < MAX.x && a.y >= 0 && a.y < MAX.y;
}

std::vector<Pos> calc_antis(Pos a, Pos b, bool limit_antis) {
	std::vector<Pos> p;
	auto diff = Pos{a.x - b.x, a.y - b.y};
	auto anti = a;
	while (in_bounds(anti)) {
		anti = {anti.x + diff.x, anti.y + diff.y};
		p.push_back(anti);
		if (limit_antis)
			break;
	}
	if (!limit_antis) {
		p.push_back(a);
	}
	return p;
}

auto parse(std::string_view filename) {
	std::map<char, std::vector<Pos>> m;

	auto y = 0;
	std::ifstream file{std::string(filename)};
	for (std::string line; std::getline(file, line);) {
		MAX.x = line.length();
		auto pos = 0;

		while (true) {
			pos = line.find_first_not_of(".", pos);
			if (pos != std::string_view::npos) {
				m[line[pos]].push_back(Pos{(int)pos, y});
				pos++;
			} else {
				break;
			}
		}
		y++;
	}
	MAX.y = y;

	return m;
}

void count_validnodes(std::span<Pos> poss, std::set<Pos> &antinodes, bool limit_antis) {
	for (auto a : poss) {
		for (auto b : poss) {
			if (a == b)
				continue;
			auto antis = calc_antis(a, b, limit_antis);

			for (auto aa : antis) {
				if (in_bounds(aa)) {
					antinodes.insert(aa);
				}
			}
		}
	}
}

int main(int argc, char *argv[]) {

	const auto nts = parse(argv[1]);

	{
		std::set<Pos> antinodes;
		for (auto [chr, poss] : nts) {
			count_validnodes(poss, antinodes, true);
		}

		// print_grid(antinodes, nts);
		std::cout << "Part 1: " << antinodes.size() << "\n";
	}
	//299

	{
		std::set<Pos> antinodes;
		for (auto [chr, poss] : nts) {
			count_validnodes(poss, antinodes, false);
		}

		// print_grid(antinodes, nts);
		std::cout << "Part 2: " << antinodes.size() << "\n";
	}
	// 1032

	return 0;
}

void print_grid(std::set<Pos> antinodes, std::map<char, std::vector<Pos>> map) {
	std::vector<std::vector<char>> grid;

	grid.resize(MAX.y);
	for (auto &line : grid) {
		line.resize(MAX.x, '.');
	}

	for (auto anti : antinodes) {
		grid[anti.y][anti.x] = '#';
	}

	for (auto [freq, ant] : map) {
		for (auto an : ant) {
			grid[an.y][an.x] = freq;
		}
	}
	for (auto y = 0; y < MAX.y; y++) {
		for (auto x = 0; x < MAX.x; x++) {
			std::cout << grid[y][x];
		}

		std::cout << "\n";
	}
}
