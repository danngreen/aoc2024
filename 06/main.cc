#include "../common/fileparse.hh"
#include <iostream>
#include <numeric>

void print_grid(std::vector<std::string> const &grid);

struct Guard {
	int x;
	int y;
	char dir;
	// int next_x;
	// int next_y;
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

bool step(std::vector<std::string> &grid, Guard &guard) {

	auto [dx, dy] = next(guard.dir);
	auto test_x = guard.x + dx;
	auto test_y = guard.y + dy;

	auto in_bounds_x = std::clamp<int>(test_x, 0, grid.front().size() - 1) == test_x;
	auto in_bounds_y = std::clamp<int>(test_y, 0, grid.front().size() - 1) == test_y;
	if (!in_bounds_x || !in_bounds_y) {
		grid[guard.y][guard.x] = guard.dir;
		return false;
	}

	if (grid[test_y][test_x] != '#') {
		grid[guard.y][guard.x] = guard.dir;
		guard.x = test_x;
		guard.y = test_y;
	} else {
		guard.dir = rotate(guard.dir);
	}
	grid[guard.y][guard.x] = guard.dir;

	return true;
}

size_t count_xs(std::vector<std::string> const &grid) {
	return std::accumulate(grid.begin(), grid.end(), 0u, [](size_t sum, std::string const &line) {
		return sum + std::count(line.begin(), line.end(), '^') + std::count(line.begin(), line.end(), '<') +
			   std::count(line.begin(), line.end(), '>') + std::count(line.begin(), line.end(), 'v');
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

	return 0;
}

void print_grid(std::vector<std::string> const &grid) {
	for (auto &line : grid)
		std::cout << line << "\n";
	std::cout << "\n";
}
