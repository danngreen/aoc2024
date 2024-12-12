#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

using Stones = std::map<long, long>;

void print_stones(Stones stones);

Stones parse(std::string filename) {
	std::ifstream ifs(std::string{filename});
	std::string line(std::istreambuf_iterator<char>{ifs}, {});

	auto nums = line									 //
			  | std::views::split(' ')					 // split on spaces
			  | std::views::transform([](auto &&range) { // convert to int
					return std::stol(std::string{range.begin(), range.end()});
				});

	std::vector<long> stonelist(nums.begin(), nums.end());

	Stones stones;
	for (auto stone : stonelist)
		stones.insert({stone, 1});

	return stones;
}

constexpr unsigned num_digits(long long x) {
	unsigned digs = 0;
	while (x > 0) {
		x /= 10;
		digs++;
	}
	return digs;
}

// using this instead of std::pow(10,x) so `split()` can be constexpr in gcc-14
constexpr long pow10(long x) {
	long tens = 1;
	while (x--)
		tens *= 10;
	return tens;
}

constexpr std::pair<long, long> split(long x) {
	auto num_digs = num_digits(x);
	if (num_digs % 2 == 1) {
		return {-1, -1};
	} else {
		auto mult = pow10(num_digs / 2);
		long a = x / mult;
		long b = x - a * mult;
		return {a, b};
	}
}

constexpr void increment(long x, long num, Stones &stones) {
	if (stones.contains(x))
		stones[x] = stones[x] + num;
	else
		stones[x] = num;
}

constexpr Stones blink(Stones stones) {
	Stones next;

	for (auto const &stone : stones) {
		if (stone.first == 0) {
			increment(1, stone.second, next);

		} else if (auto [a, b] = split(stone.first); a != -1) {
			increment(a, stone.second, next);
			increment(b, stone.second, next);

		} else {
			increment(stone.first * 2024, stone.second, next);
		}
	}

	return next;
}

int main(int argc, char *argv[]) {
	auto stones = parse(argv[1]);

	for (auto i = 0; i < 25; i++) {
		stones = blink(stones);
	}

	auto total =
		std::accumulate(stones.begin(), stones.end(), 0UL, [](unsigned long sum, auto &x) { return sum + x.second; });

	std::cout << "Part 1: " << total << "\n";
	// 202019

	for (auto i = 25; i < 75; i++) {
		stones = blink(stones);
	}

	auto total2 =
		std::accumulate(stones.begin(), stones.end(), 0UL, [](unsigned long sum, auto &x) { return sum + x.second; });

	std::cout << "Part 2: " << total2 << "\n";
	//239321955280205

	return 0;
}

// Tests:
static_assert(pow10(0) == 1);
static_assert(pow10(2) == 100);
static_assert(pow10(3) == 1000);

static_assert(split(1234) == std::pair<long, long>{12, 34});
static_assert(split(998) == std::pair<long, long>{-1, -1});
static_assert(split(999999) == std::pair<long, long>{999, 999});
static_assert(split(123456789012345678) == std::pair<long, long>{123456789, 12345678});

static_assert(num_digits(45) == 2);
static_assert(num_digits(457) == 3);
static_assert(num_digits(1234) == 4);

// std::map is not constexpr :(
// static_assert(blink(Stones{{0, 1}}) == Stones{{1, 1}});
// static_assert(blink(Stones{{1, 1}}) == Stones{{2048, 1}});
// static_assert(blink(Stones{{2048, 1}}) == Stones{{20, 1}, {48, 1}});
// static_assert(blink(Stones{{2048, 2}}) == Stones{{20, 2}, {48, 2}});

void print_stones(Stones stones) {
	for (auto const &s : stones) {
		std::cout << "" << s.first << "";
		if (s.second > 1)
			std::cout << "x" << s.second;
		std::cout << " ";
	}
	std::cout << "\n";
}
