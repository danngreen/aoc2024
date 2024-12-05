#include "../common/fileparse.hh"
#include <algorithm>
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
							 return to_int({range.begin(), range.end()});
						 });

			if (!pages.empty()) {
				updates.push_back({pages.begin(), pages.end()});
			}
		}
	}

	return {updates, rules};
}

bool check_update(std::vector<int> const &update, std::vector<Rule> const &rules) {
	for (auto rule : rules) {
		auto pos_first = std::ranges::find(update, rule.first);
		auto pos_second = std::ranges::find(update, rule.second);

		// there is a rule for this update, and we violate it: fail
		if (pos_first != update.end() && pos_second != update.end() && pos_first > pos_second) {
			return false;
		}
	}

	return true;
}

bool fix_update(std::vector<int> &update, std::vector<Rule> const &rules) {
	bool did_sort = false;

	// repeatedly apply rules until no more sorting can be done
	while (true) {
		bool fully_sorted = true;
		for (auto rule : rules) {
			auto pos_first = std::ranges::find(update, rule.first);
			auto pos_second = std::ranges::find(update, rule.second);

			// there is a rule for this update, and we violate it: swap and tag this for inclusion in the answer
			if (pos_first != update.end() && pos_second != update.end() && pos_first > pos_second) {
				std::swap(*pos_first, *pos_second);
				fully_sorted = false;
				did_sort = true;
			}
		}
		if (fully_sorted)
			break;
	}

	return did_sort;
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
	//5166

	auto sum2 = std::accumulate(updates.begin(), updates.end(), 0u, [&rules](unsigned sum, auto &update) {
		return sum + (fix_update(update, rules) ? update[update.size() / 2] : 0);
	});

	std::cout << "Part 2: sum of middle incorrect updates after sorting: " << sum2 << "\n";
	//4679

	return 0;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

static_assert(parse_rule("78|53").first == 78);
static_assert(parse_rule("78|53").second == 53);
static_assert(parse_rule("78|53\n").first == 78);
static_assert(parse_rule("78|53\n").second == 53);
