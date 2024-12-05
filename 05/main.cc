#include "../common/fileparse.hh"
#include <iostream>
#include <numeric>
#include <vector>

struct Rule {
	int first;
	int second;
};

constexpr int to_int(std::string_view str) {
	return (str[0] - '0') * 10 + (str[1] - '0');
}

constexpr Rule parse_rule(std::string_view line) {
	if (auto split = line.find('|'); split != std::string_view::npos) {
		return {to_int(line.substr(0, split)), to_int(line.substr(split + 1))};
	} else
		return {0, 0};
}

constexpr std::vector<Rule> parse_rules(std::span<std::string> const &lines) {
	std::vector<Rule> rules;

	for (auto &line : lines) {
		if (auto rule = parse_rule(line); rule.first != 0) {
			rules.push_back(rule);
		}
	}

	return rules;
}

constexpr std::string_view trim_spaces(std::string_view str) {
	size_t start = str.find_first_not_of(" ");
	size_t end = str.find_last_not_of(" ");
	if (start != std::string::npos && end != std::string::npos) {
		return str.substr(start, end - start + 1);
	}
	return "";
}

using Updates = std::vector<std::vector<int>>;
using Rules = std::vector<Rule>;

std::pair<Updates, Rules> parse(std::string_view filename) {
	std::ifstream file{std::string(filename)};

	Updates updates;
	Rules rules;

	// Read each line from the file
	for (std::string line; std::getline(file, line);) {
		if (auto rule = parse_rule(line); rule.first != 0) {
			rules.push_back(rule);
		} else {
			auto pages = line											  //
					   | std::ranges::views::split(',')					  // split on spaces
					   | std::ranges::views::transform([](auto &&range) { // convert to int
							 auto trimmed = trim_spaces({range.begin(), range.end()});
							 return str_to_int(trimmed);
						 });

			if (!pages.empty()) {
				updates.push_back({pages.begin(), pages.end()});
			}
		}
	}

	return {updates, rules};
}

bool check_update(std::vector<int> const &update, std::vector<Rule> const &rules) {
	for (unsigned idx = 0; auto page : update) {

		for (auto &rule : rules) {
			std::cout << rule.first << "|" << rule.second << " ";

			if (page == rule.first) {
				// fail if rule.second occurs in update BEFORE idx
				for (unsigned j = 0; j < idx; j++) {
					if (update[j] == rule.second)
						return false;
				}
				// ??? what's wrong with this?
				// if (auto pos = std::find(update.begin(), std::next(update.begin(), idx), rule.second);
				// 	pos != update.end())

			} else if (page == rule.second) {
				// fail if rule.first occurs in update AFTER idx
				if (std::find(update.begin() + idx + 1, update.end(), rule.first) != update.end()) {
					return false;
				}
			}
		}
		idx++;
	}
	return true;
}

int main(int argc, char *argv[]) {
	auto [updates, rules] = parse(argv[1]);

	auto sum = std::accumulate(updates.begin(), updates.end(), 0u, [&rules](unsigned sum, auto const &update) {
		if (check_update(update, rules))
			return sum + update[update.size() / 2];
		else
			return sum;
	});

	std::cout << "Part 1: sum of middle correct updates: " << sum << "\n";

	// for (auto &rule : rules)
	// 	std::cout << rule.first << "|" << rule.second << "\n";

	// for (auto &update : updates) {
	// 	for (auto page : update)
	// 		std::cout << page << ", ";
	// 	std::cout << "\n";
	// }
	return 0;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

static_assert(parse_rule("78|53").first == 78);
static_assert(parse_rule("78|53").second == 53);
static_assert(parse_rule("78|53\n").first == 78);
static_assert(parse_rule("78|53\n").second == 53);

constexpr std::pair<int, std::string_view> pop_page(std::string_view line) {
	auto split = line.find(',');
	std::string_view num;
	if (split == std::string_view::npos) {
		num = line;
		line = "";
	} else {
		num = line.substr(0, split);
		line = trim_spaces(line.substr(split + 1));
	}
	return {to_int(num), line};
}

constexpr std::vector<int> parse_update(std::string_view line) {
	std::vector<int> pages;
	auto pos = 0;

	while (line != "") {
		auto [page, remaining] = pop_page(line);
		line = remaining;
		pages.push_back(page);
	}

	return pages;
}

constexpr std::vector<std::vector<int>> parse_updates(std::span<std::string> const &lines) {
	std::vector<std::vector<int>> updates;

	for (auto &line : lines) {
		if (line.find(',') != std::string::npos)
			updates.push_back(parse_update(line));
	}

	return updates;
}

static_assert(pop_page("73, 44, 20").first == 73);
static_assert(pop_page("73, 44, 20").second == "44, 20");
static_assert(pop_page("44, 20").first == 44);
static_assert(pop_page("44, 20").second == "20");
static_assert(pop_page("20").first == 20);
static_assert(pop_page("20").second == "");

static_assert(parse_update("73, 44, 30") == std::vector<int>{73, 44, 30});
