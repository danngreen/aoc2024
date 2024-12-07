#include <fstream>
#include <ranges>
#include <string>
#include <vector>

inline int str_to_int(std::string_view str) {
	return stoi(std::string(str));
}

inline std::vector<std::vector<int>> parse_rows(std::string_view filename) {
	std::ifstream file{std::string(filename)};

	std::vector<std::vector<int>> result;

	// Read each line from the file
	for (std::string line; std::getline(file, line);) {

		// Split the line into integers, skipping empty strings
		auto nums = line																  //
				  | std::ranges::views::split(' ')										  // split on spaces
				  | std::ranges::views::filter([](auto &&str) { return str.size() > 0; }) // allows for multiple spaces
				  | std::ranges::views::transform([](auto &&range) {					  // convert to int
						return str_to_int({range.begin(), range.end()});
					});

		result.push_back({nums.begin(), nums.end()});
	}

	return result;
}

inline std::vector<std::string> parse_lines(std::string_view filename) {
	std::ifstream file{std::string(filename)};

	std::vector<std::string> lines;

	for (std::string line; std::getline(file, line);) {
		lines.push_back(line);
	}

	return lines;
}

template<typename T>
inline std::vector<T> transpose_rows_to_cols(std::vector<T> const &rows) {
	auto num_rows = rows.size();
	auto num_cols = rows.front().size();

	std::vector<T> cols;
	cols.resize(num_cols);

	for (auto &row : rows) {
		for (auto [num, col] : std::views::zip(row, cols)) {
			col.push_back(num);
		}
	}

	return cols;
}
