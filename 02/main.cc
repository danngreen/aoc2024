#include "../common/fileparse.hh"
#include <algorithm>
#include <iostream>
#include <ranges>

bool row_is_safe(std::span<int> vals) {
	auto diffs = vals | std::views::adjacent_transform<2>([](auto &&x, auto &&y) { return x - y; });

	auto passes = std::ranges::fold_left(
		diffs, 0, [](int sum, int x) { return sum + (std::abs(x) <= 3 && std::abs(x) >= 1) ? 1 : 0; });
	// auto passes = diffs | std::views::acc([](auto &&x) { return std::abs(x) <= 3 && std::abs(x) >= 1; });

	return passes == vals.size();
}

int main(int argc, char *argv[]) {
	auto rows = parse_rows(argv[1]);

	std::cout << "Found " << rows.size() << " rows\n";

	size_t num_safe = 0;
	for (auto &row : rows) {
		auto safe = row_is_safe(row);
		num_safe += safe ? 1 : 0;

		for (auto i : row)
			std::cout << i << " ";
		std::cout << " => " << safe << "\n";
	}
	return 0;
}
