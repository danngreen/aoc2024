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

int main(int argc, char *argv[]) {
	auto num_right = 0;
	auto num_left = 0;
	auto num_down = 0;
	auto num_up = 0;
	auto num_upright = 0;
	auto num_downleft = 0;
	auto num_upleft = 0;
	auto num_downright = 0;

	auto rows = parse_lines(argv[1]);

	for (auto const &row : rows) {
		num_right += count_found(row, "XMAS");
		num_left += count_found(row, "SAMX");
	}

	{
		auto cols = transpose_rows_to_cols(rows);
		for (auto const &row : cols) {
			num_down += count_found(row, "XMAS");
			num_up += count_found(row, "SAMX");
		}
	}

	// Shift row N to the right N positions
	{
		std::vector<std::string> shift_r;
		for (unsigned i = 0; auto const &row : rows) {
			shift_r.push_back(shift_and_pad(row, i, row.size() * 2));
			i++;
		}
		auto cols_shift_r = transpose_rows_to_cols(shift_r);
		for (auto const &row : cols_shift_r) {
			num_downleft += count_found(row, "XMAS");
			num_upright += count_found(row, "SAMX");
		}
	}

	// Shift row N to the left N positions
	{
		std::vector<std::string> shift_l;
		for (unsigned i = 0; auto const &row : rows) {
			shift_l.push_back(shift_and_pad(row, rows.size() - i, row.size() * 2));
			i++;
		}
		auto cols_shift_l = transpose_rows_to_cols(shift_l);
		for (auto const &row : cols_shift_l) {
			num_downright += count_found(row, "XMAS");
			num_upleft += count_found(row, "SAMX");
		}
	}

	auto total = num_right + num_left + num_up + num_down + num_upleft + num_upright + num_downleft + num_downright;
	std::cout << num_right << " " << num_left << " " << num_up << " " << num_down << " " << num_upleft << " "
			  << num_upright << " " << num_downleft << " " << num_downright;

	std::cout << "Part 1: total XMAS found: " << total << "\n";

	// brute force find X-MAS:

	return 0;
}
