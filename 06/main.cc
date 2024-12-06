#include "../common/fileparse.hh"
#include <algorithm>
#include <iostream>
#include <numeric>

void print_grid(std::vector<std::string> const &grid);

struct Guard {
	int x;
	int y;
	char dir;
};

constexpr std::string_view Dirs = "^>v<";

constexpr std::pair<int, int> next(char dir) {
	if (dir == 'v') {
		return {0, 1};
	} else if (dir == '^') {
		return {0, -1};
	} else if (dir == '<') {
		return {-1, 0};
	} else if (dir == '>') {
		return {1, 0};
	} else
		return {0, 0};
}

constexpr std::pair<int, int> next(Guard guard) {
	auto [dx, dy] = next(guard.dir);
	return {guard.x + dx, guard.y + dy};
}

constexpr char rotate(char dir) {
	return Dirs[(Dirs.find(dir) + 1) % 4];
}

static_assert(rotate('v') == '<');
static_assert(rotate('<') == '^');
static_assert(rotate('^') == '>');
static_assert(rotate('>') == 'v');

Guard find_guard(std::vector<std::string> const &grid) {
	for (int row = 0; auto &line : grid) {

		if (int col = line.find_first_of(Dirs); col != std::string::npos) {
			Guard guard{.x = col, .y = row, .dir = line[col]};
			return guard;
		}
		row++;
	}
	return {};
}

bool in_bounds(int x, int y, auto const &grid) {
	auto in_bounds_x = std::clamp<int>(x, 0, grid.front().size() - 1) == x;
	auto in_bounds_y = std::clamp<int>(y, 0, grid.size() - 1) == y;
	return in_bounds_y && in_bounds_x;
}

bool is_path_marker(char x) {
	return Dirs.find_first_of(x) != std::string_view::npos;
}

enum class Status { Continue, Looped, Exited };

Status step(std::vector<std::string> &grid, Guard &guard) {

	auto [test_x, test_y] = next(guard);

	if (!in_bounds(test_x, test_y, grid)) {
		grid[guard.y][guard.x] = guard.dir;
		return Status::Exited;
	}

	if (grid[test_y][test_x] == guard.dir) {
		return Status::Looped;
	}

	if (grid[test_y][test_x] != '#') {
		guard.x = test_x;
		guard.y = test_y;
		grid[guard.y][guard.x] = guard.dir;
	} else {
		guard.dir = rotate(guard.dir);
		// grid[guard.y][guard.x] = guard.dir;
	}

	return Status::Continue;
}

size_t count_path_markers(std::vector<std::string> const &grid) {
	return std::accumulate(grid.begin(), grid.end(), 0u, [](size_t sum, std::string const &line) {
		return sum + std::ranges::count_if(line, [](char x) { return is_path_marker(x); });
	});
}

int count_loops(std::vector<std::string> const &init_grid,
				Guard const init_guard,
				std::vector<std::string> const path_grid) {
	// Naive method: try putting an obstacle on each space and see
	// if we loop or exit
	unsigned looped = 0;
	for (unsigned x = 0; x < init_grid.front().size(); x++) {
		for (unsigned y = 0; y < init_grid.size(); y++) {
			// rule: can't place obstacle on guard init position
			if (x == init_guard.x && y == init_guard.y)
				continue;

			// only check path followed in part 1
			if (!is_path_marker(path_grid[y][x]))
				continue;

			auto grid2 = init_grid;
			auto guard2 = init_guard;

			grid2[y][x] = '#';

			while (true) {
				if (auto status = step(grid2, guard2); status == Status::Looped) {
					looped++;
					break;

				} else if (status == Status::Exited) {
					break;
				}
			}
		}
	}

	return looped;
}

int main(int argc, char *argv[]) {
	const auto grid = parse_lines(argv[1]);
	const auto guard = find_guard(grid);

	auto grid1 = grid;
	auto guard1 = guard;
	while (step(grid1, guard1) == Status::Continue) {
		// print_grid(grid1);
	}

	auto num_xs = count_path_markers(grid1);

	std::cout << "Part 1: " << num_xs << "\n";
	//4982

	auto num_loops = count_loops(grid, guard, grid1);

	std::cout << "Part 2: " << num_loops << "\n";
	//1663

	return 0;
}

void print_grid(std::vector<std::string> const &grid) {
	for (auto &line : grid)
		std::cout << line << "\n";
	std::cout << "\n";
}
