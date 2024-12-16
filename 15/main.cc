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

struct State {
	Vec pos;
	Dir dir;
	unsigned score{};
};

std::vector<unsigned> run_paths(State start, Vec end) {
	std::vector<std::vector<bool>> visited(input.front().size(), std::vector<bool>(input.size(), 0));

	std::vector<unsigned> scores{};

	std::deque<State> branches;
	branches.push_front(start);

	while (!branches.empty()) {
		auto cur = branches.front();
		branches.pop_front();

		std::cout << "At " << cur.pos.x << "," << cur.pos.y << ": " << cur.dir << " (" << cur.score << ")\n";

		if (cur.pos == end) {
			std::cout << "End!\n";
			scores.push_back(cur.score);
			continue;
		}

		if (visited[cur.pos.y][cur.pos.x])
			continue;
		visited[cur.pos.y][cur.pos.x] = true;

		for (auto next_dir : {N, S, W, E}) {
			// auto next_dir = Dir((cur.dir + dir) % 4);
			auto next_pos = cur.pos + Dirs[next_dir];

			if (input[next_pos.y][next_pos.x] != '#') {

				// try current dir next:
				if (next_dir == cur.dir) {
					branches.push_front(State{next_pos, next_dir, cur.score + 1});
				} else {
					branches.push_back(State{next_pos, next_dir, cur.score + 1001});
				}
				// auto next_score = cur.score + ((next_dir == cur.dir) ? 1 : 1001);
				// branches.push_back(State{next_pos, next_dir, next_score});
			}
		}
	}
	return scores;
}

int main(int argc, char *argv[]) {
	State start = {Vec{1, input.size() - 2}, Dir::E};
	Vec end = {input.front().size() - 2, 1};

	auto score = run_paths(start, end);

	std::cout << "Part 1: " << *std::ranges::min_element(score) << "\n";
	// 92440 too high

	// std::cout << "Part 2: " << 7502 << "\n";

	return 0;
}

void print(std::span<std::string_view> map) {
	for (auto &line : map) {
		std::cout << line << "\n";
	}
	std::cout << "\n";
}
