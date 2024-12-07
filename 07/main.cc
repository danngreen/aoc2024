#include "../common/fileparse.hh"
#include <iostream>
#include <numeric>
#include <ranges>

enum Op { Unknown, Plus, Mult, Concat };

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
	else if (op == Op::Concat)
		return a + b;
	else
		return 0; //error
}

constexpr long solve(Equation eq) {
	long ans = eq.initial;
	for (auto [num, op] : std::views::zip(eq.nums, eq.ops)) {
		ans = calc(ans, num, op);
	}
	return ans;
}

constexpr bool check(Equation eq) {
	return solve(eq) == eq.answer;
}

// returns if can be solved
bool find_ops(Equation &eq) {
	eq.ops.resize(eq.nums.size());

	for (unsigned mask = 0; mask < (1 << eq.nums.size()); mask++) {
		for (unsigned bit = 0; auto &op : eq.ops) {
			op = mask & (1 << bit) ? Op::Plus : Op::Mult;
			bit++;
		}
		if (check(eq))
			return true;
	}

	return false;
}

// static_assert(solve({42, 2, {40}, {Op::Plus}}) == 42);

std::vector<Equation> parse(std::string_view filename) {
	std::vector<Equation> eqs;

	std::ifstream file{std::string(filename)};

	for (std::string line; std::getline(file, line);) {
		Equation eq;
		auto colpos = line.find(':');
		try {
			eq.answer = stol(line.substr(0, colpos));
		} catch (std::exception &e) {
			std::cout << line.substr(0, colpos) << " out of range\n";
		}
		line = line.substr(colpos + 2);

		auto pos = line.find(' ');
		eq.initial = stol(line.substr(0, pos));
		line = line.substr(pos + 1);

		while (true) {
			pos = line.find(' ');
			eq.nums.push_back(stol(line.substr(0, pos)));
			line = line.substr(pos + 1);

			if (pos == std::string_view::npos)
				break;

			eq.ops.push_back(Op::Unknown);
		}

		eqs.push_back(eq);
	}

	return eqs;
}

int main(int argc, char *argv[]) {

	auto eqs = parse(argv[1]);

	// ???
	// long good_sum = std::accumulate(
	// 	eqs.begin(), eqs.end(), 0l, [](long sum, Equation &eq) { return sum + (find_ops(eq) ? eq.answer); });

	long sum = 0;
	for (auto &eq : eqs) {
		// eq.print();
		if (find_ops(eq)) {
			sum += eq.answer;
		}
	}

	std::cout << "Part 1: " << sum << "\n";
	//6083020304036

	// std::cout << "Part 2: " << num_loops << "\n";

	return 0;
}

void Equation::print() {
	std::cout << answer << ": " << initial << " ";
	for (auto num : nums)
		std::cout << num << " ";
	std::cout << "\n";
}
