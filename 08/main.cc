#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <span>
#include <vector>

struct Pos {
	int x;
	int y;
	bool operator<(Pos const &other) const {
		return x < other.x || (x == other.x && y < other.y);
	}
};

Pos MAX;

std::pair<Pos, Pos> calc_antis(Pos a, Pos b) {
	return {Pos{2 * b.x - a.x, 2 * b.y - a.y}, Pos{2 * a.x - b.x, 2 * a.y - b.y}};
}

bool in_bounds(Pos a) {
	return a.x >= 0 && a.x < MAX.x && a.y >= 0 && a.y < MAX.y;
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

void count_validnodes(std::span<Pos> poss, std::vector<Pos> &antinodes) {
	// void count_validnodes(std::span<Pos> poss, std::set<Pos> &antinodes) {
	if (poss.size() < 2)
		return;

	int cnt = 0;
	auto a = poss.front();
	auto check = poss.subspan(1);
	for (auto b : check) {
		auto [aa, bb] = calc_antis(a, b);

		if (in_bounds(aa)) {
			if (std::find_if(antinodes.begin(), antinodes.end(), [&aa](auto &bb) {
					return aa.x == bb.x && aa.y == bb.y;
				}) == std::end(antinodes))
			{
				antinodes.push_back(aa);
				std::cout << aa.x << "," << aa.y << "\n";
			}
			// auto [_, unique] = antinodes.insert(aa);
			// if (!unique)
			// 	std::cout << aa.x << "," << aa.y << "\n";
		}
		if (in_bounds(bb)) {
			if (std::find_if(antinodes.begin(), antinodes.end(), [&bb](auto &aa) {
					return aa.x == bb.x && aa.y == bb.y;
				}) == std::end(antinodes))
			{
				antinodes.push_back(bb);
				std::cout << bb.x << "," << bb.y << "\n";
			}
			// antinodes.insert(bb);
			// auto [_, unique] = antinodes.insert(aa);
			// if (!unique)
			// 	std::cout << bb.x << "," << bb.y << "\n";
		}
	}
	count_validnodes(check, antinodes);
}

int main(int argc, char *argv[]) {

	auto nts = parse(argv[1]);

	// for (auto nt : nts) {
	// 	std::cout << nt.first << "@ ";
	// 	for (auto xy : nt.second)
	// 		std::cout << xy.x << "," << xy.y << " ";
	// 	std::cout << "\n";
	// }
	// std::set<Pos> antinodes;
	std::vector<Pos> antinodes;
	for (auto [chr, poss] : nts) {
		count_validnodes(poss, antinodes);
	}

	std::cout << "Part 1: " << antinodes.size() << "\n";
	//299

	// std::cout << "Part 2: " << sum2 << "\n";
	//59002246504791

	//>242? <305

	return 0;
}
