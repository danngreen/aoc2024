#include "input.hh"
#include "vec.hh"
#include <iostream>
#include <span>
#include <vector>

constexpr RatVec solve(Vec A, Vec B, Vec prize) {
	auto det = A.x * B.y - A.y * B.x;

	return {{(prize.x * B.y - prize.y * B.x), (prize.y * A.x - prize.x * A.y)}, {det, det}};
}

constexpr long calc_tokens(std::span<const Input> input) {
	long tokens = 0;

	for (auto const &in : input) {
		auto [n, d] = solve(in.A, in.B, in.prize);
		Vec solution = n / d;

		// Check if solution is integral
		if (solution * d == n) {
			// std::cout << "Press A " << X.x << " Press B " << X.y << "\n";
			tokens += solution.x * 3 + solution.y;
		}
	}

	return tokens;
}

constexpr auto add_ten_trillion = [](std::span<const Input> input) {
	std::vector<Input> input2;
	input2.resize(input.size());
	std::copy(input.begin(), input.end(), input2.begin());
	for (auto &i : input2) {
		i.prize.x += 10000000000000;
		i.prize.y += 10000000000000;
	}
	return input2;
};

// Part 1:
static_assert(calc_tokens(input) == 40369);

// Part 2:
static_assert(calc_tokens(add_ten_trillion(input)) == 72587986598368);

/// Test input:

constexpr std::array test_input = {
	Input{{94, 34}, {22, 67}, {8400, 5400}},
	Input{{26, 66}, {67, 21}, {12748, 12176}},
	Input{{17, 86}, {84, 37}, {7870, 6450}},
	Input{{69, 23}, {27, 71}, {18641, 10279}},
};

static_assert(calc_tokens(test_input) == 480);

int main(int argc, char *argv[]) {
	std::cout << "Part 1: " << calc_tokens(input) << "\n";
	std::cout << "Part 2: " << calc_tokens(add_ten_trillion(input)) << "\n";
	return 0;
}
