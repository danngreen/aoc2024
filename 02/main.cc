#include "../common/fileparse.hh"
#include <algorithm>
#include <iostream>
#include <ranges>

bool row_is_safe(std::span<int> vals) {
	auto diffs = vals | std::views::adjacent_transform<2>([](auto &&x, auto &&y) { return x - y; });

	for (auto i : vals)
		std::cout << i << " ";
	std::cout << " => ";
	for (auto i : diffs)
		std::cout << i << " ";

	auto passes = std::ranges::fold_left(diffs, 0, [](int sum, int x) {
		int is_ok = (std::abs(x) <= 3 && std::abs(x) >= 1);
		return sum + (is_ok ? 1 : 0);
	});

	std::cout << " => " << passes << " = " << (passes == vals.size() - 1) << "\n";

	return passes == vals.size() - 1;
}

int main(int argc, char *argv[]) {
	auto rows = parse_rows(argv[1]);

	std::cout << "Found " << rows.size() << " rows\n";

	size_t num_safe = 0;
	for (auto &row : rows) {
		auto safe = row_is_safe(row);
		num_safe += safe ? 1 : 0;
	}
	std::cout << "Total Safe: " << num_safe << "\n";
	return 0;
}
