#include "../common/fileparse.hh"
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

//actually `obstacles` is a vec of places where we would rotate if there was an obstacle in the next spot
std::vector<std::pair<int, int>> obstacles;

bool in_bounds(int x, int y, auto const &grid) {
	auto in_bounds_x = std::clamp<int>(x, 0, grid.front().size() - 1) == x;
	auto in_bounds_y = std::clamp<int>(y, 0, grid.size() - 1) == y;
	return in_bounds_y && in_bounds_x;
}

bool obstacle_would_loop(std::vector<std::string> grid, Guard guard);

bool step(std::vector<std::string> &grid, Guard &guard, bool check_obs = true) {

	auto [test_x, test_y] = next(guard);

	if (!in_bounds(test_x, test_y, grid)) {
		grid[guard.y][guard.x] = guard.dir;
		return false;
	}

	if (grid[test_y][test_x] != '#') {
		if (check_obs) {
			Guard test_guard{test_x, test_y, rotate(guard.dir)};
			if (obstacle_would_loop(grid, test_guard)) {
				auto [obstx, obsty] = next(Guard{test_x, test_y, guard.dir});
				obstacles.push_back({obstx, obsty});
				std::cout << "______________________" << "\n";
				std::cout << obstx << " " << obsty << "\n";
				auto c = grid[obsty][obstx];
				grid[obsty][obstx] = 'O';
				print_grid(grid);
				grid[obsty][obstx] = c;
			}
		}
		guard.x = test_x;
		guard.y = test_y;
		grid[guard.y][guard.x] = guard.dir;
	} else {
		guard.dir = rotate(guard.dir);
		grid[guard.y][guard.x] = guard.dir;
	}

	return true;
}

bool obstacle_would_loop(std::vector<std::string> grid, Guard guard) {
	grid[guard.y][guard.x] = guard.dir;

	std::cout << "############################" << "\n";
	while (true) {
		print_grid(grid);
		auto [dx, dy] = next(guard.dir);
		auto test_x = guard.x + dx;
		auto test_y = guard.y + dy;

		if (!in_bounds(test_x, test_y, grid)) {
			std::cout << "No loop\n";
			return false;
		}

		if (grid[test_y][test_x] == guard.dir) {
			std::cout << "Loop\n";
			return true;
		}

		if (grid[test_y][test_x] != '#') {
			guard.x = test_x;
			guard.y = test_y;
			grid[guard.y][guard.x] = guard.dir;
		} else {
			guard.dir = rotate(guard.dir);
		}
	}

	return false;
}

size_t count_xs(std::vector<std::string> const &grid) {
	return std::accumulate(grid.begin(), grid.end(), 0u, [](size_t sum, std::string const &line) {
		return sum										 //
			 + std::count(line.begin(), line.end(), '^') //
			 + std::count(line.begin(), line.end(), '<') //
			 + std::count(line.begin(), line.end(), '>') //
			 + std::count(line.begin(), line.end(), 'v');
	});
}

// as we go, check the next position as if we had rotated in our current position and then stepped
// if it's a place we've been and were going in that new rotated dir, then our current next step is a valid obstacle.

int main(int argc, char *argv[]) {
	auto grid = parse_lines(argv[1]);
	auto guard = find_guard(grid);

	while (step(grid, guard)) {
		// print_grid(grid);
	}

	auto num_xs = count_xs(grid);

	std::cout << "Part 1: " << num_xs << "\n";
	//4982

	std::cout << "Part 2: " << obstacles.size() << "\n";
	//<1687

	return 0;
}

void print_grid(std::vector<std::string> const &grid) {
	for (auto &line : grid)
		std::cout << line << "\n";
	std::cout << "\n";
}
