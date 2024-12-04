#include "../common/fileparse.hh"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

constexpr unsigned count_found(std::string_view haystack, std::string_view needle) {
	unsigned num_found = 0;
	size_t pos = 0;
	while (true) {
		auto offset = haystack.find(needle, pos);
		if (offset == std::string::npos)
			break;
		num_found++;
		pos = offset + needle.length();
	}
	return num_found;
}
static_assert(std::string_view{"XMASX"}.find("XMAS", 0) == 0);
static_assert(std::string_view{"XMAS"}.length() == 4);
static_assert(std::string_view{"XMASX"}.find("XMAS", 4) == std::string::npos);

static_assert(count_found("XMA", "XMAS") == 0);
static_assert(count_found("XMASX", "XMAS") == 1);
static_assert(count_found("XMASXMAS", "XMAS") == 2);

static_assert(count_found("XMMASSSMAMASXMAMASXMAS", "XMAS") == 1);
static_assert(count_found("XMMASSSMAXMASXMAMASXMAS", "XMAS") == 2);
static_assert(count_found("XMASXMASXMAS", "XMAS") == 3);

constexpr std::string shift_and_pad(std::string const &str, unsigned shift, size_t ending_size, char padchar = ' ') {
	size_t left_pad = shift;
	size_t right_pad = ending_size - str.length() - left_pad;

	std::string left(left_pad, padchar);
	std::string right(right_pad, padchar);
	return left + str + right;
}

static_assert(shift_and_pad("ABC", 0, 3, ' ') == "ABC");
static_assert(shift_and_pad("ABC", 0, 6, ' ') == "ABC   ");
static_assert(shift_and_pad("ABC", 1, 6, ' ') == " ABC  ");
static_assert(shift_and_pad("ABC", 2, 6, ' ') == "  ABC ");
static_assert(shift_and_pad("ABC", 3, 6, ' ') == "   ABC");

std::pair<int, int> find_in_rows(auto &&rows) {
	auto num_right = 0;
	auto num_left = 0;

	for (auto const &row : rows) {
		num_right += count_found(row, "XMAS");
		num_left += count_found(row, "SAMX");
	}

	return {num_right, num_left};
}

unsigned find_xs(std::vector<std::string> grid) {

	auto numrows = grid.size();
	auto numcols = grid.front().length();

	unsigned found = 0;

	for (unsigned y = 1; y < numrows - 1; y++) {
		for (unsigned x = 1; x < numcols - 1; x++) {
			if (grid[y][x] == 'A') {
				//M M  M S  S S  S M
				// A    A    A    A
				//S S  M S  M M  S M
				if (grid[y - 1][x - 1] == 'M' && grid[y + 1][x + 1] == 'S') {
					if (grid[y - 1][x + 1] == 'M' && grid[y + 1][x - 1] == 'S') {
						found++;
					}
					if (grid[y - 1][x + 1] == 'S' && grid[y + 1][x - 1] == 'M') {
						found++;
					}
				}
				if (grid[y - 1][x - 1] == 'S' && grid[y + 1][x + 1] == 'M') {
					if (grid[y - 1][x + 1] == 'M' && grid[y + 1][x - 1] == 'S') {
						found++;
					}
					if (grid[y - 1][x + 1] == 'S' && grid[y + 1][x - 1] == 'M') {
						found++;
					}
				}
			}
		}
	}
	return found;
}

int main(int argc, char *argv[]) {

	auto rows = parse_lines(argv[1]);

	auto [num_right, num_left] = find_in_rows(rows);

	auto cols = transpose_rows_to_cols(rows);
	auto [num_down, num_up] = find_in_rows(cols);

	// Shift row N to the right N positions
	std::vector<std::string> shift_r;
	for (unsigned i = 0; auto const &row : rows) {
		shift_r.push_back(shift_and_pad(row, i, row.size() * 2));
		i++;
	}
	auto cols_shift_r = transpose_rows_to_cols(shift_r);
	auto [num_downleft, num_upright] = find_in_rows(cols_shift_r);

	// Shift row N to the left N positions
	std::vector<std::string> shift_l;
	for (unsigned i = 0; auto const &row : rows) {
		shift_l.push_back(shift_and_pad(row, rows.size() - i, row.size() * 2));
		i++;
	}
	auto cols_shift_l = transpose_rows_to_cols(shift_l);
	auto [num_downright, num_upleft] = find_in_rows(cols_shift_l);

	auto total = num_right + num_left + num_up + num_down + num_upleft + num_upright + num_downleft + num_downright;

	std::cout << "Part 1: total XMAS found: " << total << "\n";
	//2562

	// brute force find X-MAS:
	auto num_xs = find_xs(rows);
	std::cout << "Part 2: total X-MAS found: " << num_xs << "\n";

	return 0;
}
