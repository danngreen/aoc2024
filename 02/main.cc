#include "../common/fileparse.hh"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <ranges>

constexpr std::vector<int> calc_diffs(auto &&vals) {
	std::vector<int> diffs(vals.size() - 1);
	std::transform(vals.begin(), vals.end() - 1, vals.begin() + 1, diffs.begin(), [](int x, int y) { return y - x; });
	return diffs;
}

static_assert(calc_diffs(std::array{99, 101, 104, 104}) == std::vector<int>{2, 3, 0});
static_assert(calc_diffs(std::array{-10, -10, -11, -10}) == std::vector<int>{0, -1, 1});

constexpr bool all_or_none_positive(auto &&diffs) {
	auto num_pos = std::ranges::count_if(diffs, [](int x) { return x > 0; });
	return num_pos == 0 || num_pos == diffs.size();
}

static_assert(all_or_none_positive(std::array{1, 2, 3}));
static_assert(all_or_none_positive(std::array{0, 0, 0}));
static_assert(all_or_none_positive(std::array{-1, 0, -3}));
static_assert(all_or_none_positive(std::array{-1, -2, -3}));
static_assert(all_or_none_positive(std::array{1, 2, 0}) == false);
static_assert(all_or_none_positive(std::array{1, 2, -3}) == false);

template<int Min, int Max>
constexpr bool all_in_abs_range(auto &&diffs) {
	auto num_pos =
		std::ranges::count_if(diffs, [](int x) { return (x >= Min && x <= Max) || (-x >= Min && -x <= Max); });
	return num_pos == diffs.size();
}

static_assert(all_in_abs_range<1, 3>(std::array{1, 2, -2, -1, -3, 3}));
static_assert(all_in_abs_range<1, 3>(std::array{3, 2, -1, -4}) == false);
static_assert(all_in_abs_range<1, 3>(std::array{0, 2, -1, -1}) == false);

constexpr bool row_is_safe(auto &&vals) {
	auto diffs = calc_diffs(vals);

	auto all_same_sign_or_zero = all_or_none_positive(diffs);

	auto all_in_range = all_in_abs_range<1, 3>(diffs);

	return all_same_sign_or_zero && all_in_range;
}

static_assert(row_is_safe(std::array{1, 2, 3}));
static_assert(row_is_safe(std::array{-1, -2, -3, -6}));
static_assert(row_is_safe(std::array{99, 101, 104}));
static_assert(row_is_safe(std::array{1, 2, -3}) == false);
static_assert(row_is_safe(std::array{99, 101, 104, 104}) == false);
static_assert(row_is_safe(std::array{99, 99, 104, 105}) == false);
static_assert(row_is_safe(std::array{99, 101, 104, 103}) == false);

static_assert(row_is_safe(std::array{1, 3, 6, 7, 9}));
static_assert(row_is_safe(std::array{8, 6, 4, 4, 1}) == false);
static_assert(row_is_safe(std::array{1, 3, 2, 4, 5}) == false);
static_assert(row_is_safe(std::array{9, 7, 6, 2, 1}) == false);
static_assert(row_is_safe(std::array{7, 6, 4, 2, 1}));

int main(int argc, char *argv[]) {
	auto rows = parse_rows(argv[1]);

	size_t num_safe = std::ranges::count_if(rows, [](auto &row) { return row_is_safe(row); });

	std::cout << "Total Safe: " << num_safe << "\n";
	// 407
	return 0;
}
