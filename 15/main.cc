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

enum Dir { E, S, W, N, None };
constexpr std::array<Vec, 4> Dirs{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

constexpr Dir opposing(Dir b) {
	return Dir(((int)b + 2) % 4);
}
constexpr bool parallel(Dir a, Dir b) {
	return (a == b) || (a == opposing(b));
}

struct State {
	Vec pos;
	Dir dir;
	unsigned score{};
};

std::vector<unsigned> run_paths(State start, Vec end) {
	std::vector<std::vector<Dir>> visited(input.front().size(), std::vector<Dir>(input.size(), Dir::None));

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

		// Allow visiting the same tile twice if we are heading in cross direction
		// TODO: would it make sense to allow a lower-score state to visit an already visited tile?
		if (parallel(visited[cur.pos.y][cur.pos.x], cur.dir))
			continue;

		visited[cur.pos.y][cur.pos.x] = cur.dir;

		for (auto dir : {E, S, W, N}) {
			// Don't allow U-turns
			if (dir == opposing(cur.dir))
				continue;

			auto next_pos = cur.pos + Dirs[dir];

			if (input[next_pos.y][next_pos.x] != '#') {
				// Prioritize staying on the current path
				// This weeds out an edge case where we go the "long" way
				// around a loop
				if (dir == cur.dir) {
					branches.push_front(State{next_pos, dir, cur.score + 1});
				} else {
					branches.push_back(State{next_pos, dir, cur.score + 1001});
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

	std::cout << "Part 1: " << *std::ranges::min_element(score) << "\n";
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
