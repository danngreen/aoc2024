#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

std::string read_file(const char *filename) {
	size_t filesize = std::filesystem::file_size(filename);

	auto fil = std::fopen(filename, "r");
	std::string input;
	input.resize(filesize);
	std::fread(input.data(), sizeof(char), filesize, fil);
	std::fclose(fil);

	return input;
}

int str_to_int(std::string_view str) {
	return stoi(std::string(str));
}

void parse(std::string_view data, std::vector<int> *a, std::vector<int> *b) {
	while (data.length() > 0) {
		auto astr = data.substr(0, 5);
		a->push_back(str_to_int(astr));

		auto bstr = data.substr(8, 5);
		b->push_back(str_to_int(bstr));

		data = data.substr(14);
	}
}

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
	auto input = read_file(argv[1]);

	std::vector<int> lista;
	std::vector<int> listb;

	parse(input, &lista, &listb);

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
}
