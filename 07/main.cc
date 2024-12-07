#include "../common/fileparse.hh"
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>

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
	} else
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

	for (long mask = 0; mask < std::pow(N, eq.nums.size()); mask++) {
		for (long op_idx = 0; auto &op : eq.ops) {
			op = Op(long(mask / (long)std::pow(N, op_idx)) % N);
			op_idx++;
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
		if (find_ops<2>(eq)) {
			sum += eq.answer;
		}
	}

	std::cout << "Part 1: " << sum << "\n";
	//6083020304036

	long sum2 = 0;
	for (auto &eq : eqs) {
		if (find_ops<3>(eq)) {
			sum2 += eq.answer;
		}
	}
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
