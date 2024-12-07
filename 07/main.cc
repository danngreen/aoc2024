#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

enum Op { Plus = 0, Mult = 1, Concat = 2, Unknown };

struct Equation {
	long answer{};
	long initial{};
	std::vector<long> nums;
	std::vector<Op> ops;

	void print();
};

long calc(long a, long b, Op op) {
	if (op == Op::Mult)
		return a * b;
	else if (op == Op::Plus)
		return a + b;
	else if (op == Op::Concat) {
		auto str = std::to_string(a) + std::to_string(b);
		return stol(str);
	}
	return 0; //error
}

constexpr bool check(Equation eq) {
	long ans = eq.initial;
	for (auto [num, op] : std::views::zip(eq.nums, eq.ops)) {
		ans = calc(ans, num, op);
	}
	return ans == eq.answer;
}

// returns true if can be solved
template<size_t N>
bool find_ops(Equation &eq) {
	eq.ops.resize(eq.nums.size());

	// base-N "bitmask" to find all combinations
	long num_combos = std::pow(N, eq.nums.size());
	for (long combo = 0; combo < num_combos; combo++) {
		for (long shift = 1; auto &op : eq.ops) {
			op = Op(long(combo / shift) % N);
			shift *= N;
		}
		if (check(eq))
			return true;
	}

	return false;
}

std::vector<Equation> parse(std::string_view filename) {
	std::vector<Equation> eqs;

	std::ifstream file{std::string(filename)};

	for (std::string line; std::getline(file, line);) {
		Equation eq;

		auto colpos = line.find(':');
		eq.answer = stol(line.substr(0, colpos));
		line = line.substr(colpos + 2);

		auto nums = line											 //
				  | std::ranges::views::split(' ')					 // split on spaces
				  | std::ranges::views::transform([](auto &&range) { // string->long
						return stol(std::string{range.begin(), range.end()});
					});

		eq.initial = nums.front();
		eq.nums.append_range(nums | std::views::drop(1));

		eqs.push_back(eq);
	}

	return eqs;
}

int main(int argc, char *argv[]) {
	auto eqs = parse(argv[1]);

	long sum = std::accumulate(
		eqs.begin(), eqs.end(), 0l, [](long sum, Equation &eq) { return sum + (find_ops<2>(eq) ? eq.answer : 0); });

	std::cout << "Part 1: " << sum << "\n";
	//6083020304036

	long sum2 = std::accumulate(
		eqs.begin(), eqs.end(), 0l, [](long sum, Equation &eq) { return sum + (find_ops<3>(eq) ? eq.answer : 0); });

	std::cout << "Part 2: " << sum2 << "\n";
	//59002246504791

	return 0;
}

void Equation::print() {
	std::cout << answer << ": " << initial << " ";
	for (auto num : nums)
		std::cout << num << " ";
	std::cout << "\n";
}
