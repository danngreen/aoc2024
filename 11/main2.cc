#include <cmath>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

constexpr unsigned num_digits(long long x) {
	unsigned digs = 0;
	while (x > 0) {
		x /= 10;
		digs++;
	}
	return digs;
}

constexpr long pow10(unsigned x) {
	long tens = 1;
	while (x--)
		tens *= 10ULL;
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

struct Stone {
	unsigned times{1};
	unsigned long val{0};
	unsigned iters_left{75};
};

void blink_hard(std::vector<Stone> &stones) {

	// for (auto const &stone : stones) {
	// 	if (stone == 0)
	// 		next.push_back(1);
	// 	else if (auto [a, b] = split(stone); a != -1) {
	// 		next.push_back(a);
	// 		next.push_back(b);
	// 	} else {
	// 		next.push_back(stone * 2024);
	// 	}
	// }
}

std::vector<Stone> parse(std::string filename) {
	std::ifstream ifs(std::string{filename});
	std::string line(std::istreambuf_iterator<char>{ifs}, {});

	auto nums =
		line									   //
		| std::views::split(' ')				   // split on spaces
		| std::views::transform([](auto &&range) { // convert to int
			  return Stone{.times = 1, .val = std::stoul(std::string{range.begin(), range.end()}), .iters_left = 75};
		  });

	std::vector<Stone> stones(nums.begin(), nums.end());
	return stones;
}

void print_stone(Stone s);
void print_stones(std::span<Stone> stones);

int main(int argc, char *argv[]) {
	auto stones = parse(argv[1]);

	// 2024 = 8 * 23 * 11
	print_stones(stones);

	for (auto i = 0; i < 10; i++) {
		blink_hard(stones);
		std::cout << "[" << i << "] " << stones.size() << "\n";
		print_stones(stones);
		// long tot = 0;
		// for (auto s : stones) {
		// 	std::cout << s << " ";
		// 	tot += num_digits(s);
		// }
		// std::cout << " (" << tot << ")\n";
		// std::cout << "\n";
	}

	return 0;
	//std::cout << "Part 1: " << stones.size() << "\n";
	////20219

	// for (auto i = 25; i < 75; i++) {
	// 	stones = blink(stones);
	// 	std::cout << "[" << i << "] " << stones.size() << "\n";
	// }

	//std::cout << "Part 2: " << stones.size() << "\n";

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

// static_assert(blink(std::vector<long>{125, 17}) == std::vector<long>{253000, 1, 7});

// static_assert(blink(std::vector<long>{253000, 1, 7}) == std::vector<long>{253, 0, 2024, 14168});

// static_assert(blink(std::vector<long>{253, 0, 2024, 14168}) == std::vector<long>{512072, 1, 20, 24, 28676032});

// static_assert(blink(std::vector<long>{512072, 1, 20, 24, 28676032}) ==
// 			  std::vector<long>{512, 72, 2024, 2, 0, 2, 4, 2867, 6032});

// static_assert(blink(std::vector<long>{512, 72, 2024, 2, 0, 2, 4, 2867, 6032}) ==
// 			  std::vector<long>{1036288, 7, 2, 20, 24, 4048, 1, 4048, 8096, 28, 67, 60, 32});

// static_assert(blink(std::vector<long>{1036288, 7, 2, 20, 24, 4048, 1, 4048, 8096, 28, 67, 60, 32}) ==
// 			  std::vector<long>{2097446912, 14168, 4048, 2, 0, 2, 4, 40, 48, 2024, 40,
// 								48,			80,	   96,	 2, 8, 6, 7, 6,	 0,	 3,	   2});

void print_stone(Stone s) {
	std::cout << s.val << "{" << s.iters_left << "}";
	if (s.iters_left > 1)
		std::cout << "*" << s.iters_left;
	std::cout << " ";
}
void print_stones(std::span<Stone> stones) {
	for (auto s : stones) {
		print_stone(s);
	}
	std::cout << "\n";
}
