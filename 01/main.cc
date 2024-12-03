#include "../common/fileparse.hh"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

int count_occurrences(std::vector<int> const &vec, int target) {
	// Find the first occurence
	auto lower = std::ranges::lower_bound(vec, target);

	if (lower == vec.end() || *lower != target) {
		return 0;
	}

	// Find the position just after the last occurrence
	auto upper = std::ranges::upper_bound(vec, target);

	return upper - lower;
}

int main(int argc, char *argv[]) {
	auto rows = parse_rows(argv[1]);
	auto cols = transpose_rows_to_cols(rows);

	auto &lista = cols[0];
	auto &listb = cols[1];

	std::ranges::sort(lista);
	std::ranges::sort(listb);

	// could use std::ranges::views::zip_transform if compiler supported it
	long int sumdiff = 0;
	for (auto [a, b] : std::ranges::views::zip(lista, listb)) {
		sumdiff += std::abs(a - b);
	}

	std::cout << "Part 1 (sum of differences): " << sumdiff << "\n";

	auto similarity = std::accumulate(lista.begin(), lista.end(), 0L, [&listb](long accum, int a) {
		return accum + count_occurrences(listb, a) * a;
	});

	std::cout << "Part 2 (similarity): " << similarity << "\n";
	return 0;

	//Part 1 (sum of differences): 1341714
	//Part 2 (similarity): 27384707
}
