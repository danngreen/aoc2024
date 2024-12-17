#include "input.hh"
// #include "test_input.hh"

#include <algorithm>
#include <iostream>
#include <span>

void print(std::span<std::string_view> map);

struct Vec {
	int x{};
	int y{};

	constexpr Vec operator+(Vec const &other) const {
		return {x + other.x, y + other.y};
	}

	constexpr bool operator==(Vec const &other) const {
		return x == other.x && y == other.y;
	}
};

enum Dir { E, S, W, N };
constexpr std::array<Vec, 4> Dirs{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

constexpr Dir opposing(Dir b) {
	return Dir(((int)b + 2) % 4);
}
struct State {
	Vec pos;
	Dir dir;
	unsigned score{};
};

std::vector<int> run_paths(State start, Vec end) {
	std::vector<std::vector<unsigned>> visited(input.front().size(), std::vector<unsigned>(input.size(), 0x7FFFFFFF));
	visited[start.pos.y][start.pos.x] = -1;

	std::vector<int> scores{};

	std::deque<State> branches;
	branches.push_front(start);

	while (!branches.empty()) {
		auto cur = branches.front();
		branches.pop_front();

		std::println("At {},{}: {} ({})", cur.pos.x, cur.pos.y, std::to_underlying(cur.dir), cur.score);

		if (cur.pos == end) {
			std::println("End!");
			scores.push_back(cur.score);
			continue;
		}

		// If we've already been to this tile with a lower score, the abort this path
		if (visited[cur.pos.y][cur.pos.x] <= cur.score)
			continue;

		visited[cur.pos.y][cur.pos.x] = cur.score;

		// Check straight, +90, -90
		for (auto dir : {0, 1, 3}) {
			auto new_dir = (cur.dir + dir) % 4;
			auto next_pos = cur.pos + Dirs[new_dir];

			if (input[next_pos.y][next_pos.x] != '#') {
				// Prioritize staying on the current path
				// This weeds out an edge case where we go the "long" way
				// around a loop
				if (new_dir == cur.dir) {
					branches.push_back(State{next_pos, (Dir)new_dir, cur.score + 1});
				} else {
					branches.push_back(State{next_pos, (Dir)new_dir, cur.score + 1001});
				}
			}
		}
	}
	return scores;
}

int main(int argc, char *argv[]) {
	State start = {Vec{1, input.size() - 2}, Dir::E};
	Vec end = {input.front().size() - 2, 1};

	auto score = run_paths(start, end);

	if (score.size() > 0)
		std::println("Part 1: {}", *std::ranges::min_element(score));
	else
		std::println("No scores");
	// 92432

	// std::cout << "Part 2: " << *std::ranges::min_element(score) << "\n";

	return 0;
}

void print(std::span<std::string_view> map) {
	for (auto &line : map) {
		std::cout << line << "\n";
	}
	std::cout << "\n";
}
