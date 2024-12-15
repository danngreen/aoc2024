#include "input.hh"
#include "vec.hh"
#include <iostream>
#include <span>

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

constexpr std::array test_input = {
	Input{{94, 34}, {22, 67}, {8400, 5400}},
	Input{{26, 66}, {67, 21}, {12748, 12176}},
	Input{{17, 86}, {84, 37}, {7870, 6450}},
	Input{{69, 23}, {27, 71}, {18641, 10279}},
};

static_assert(calc_tokens(test_input) == 480);

int main(int argc, char *argv[]) {

	auto tokens = calc_tokens(input);
	std::cout << "Part 1: " << tokens << "\n";
	// 40369

	auto input2 = input;
	for (auto &in : input2) {
		in.prize.x += 10000000000000;
		in.prize.y += 10000000000000;
	}

	auto tokens2 = calc_tokens(input2);
	std::cout << "Part 2: " << tokens2 << "\n";
	// 72587986598368

	return 0;
}
