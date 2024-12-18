#include "input.hh"
#include "regs.hh"
#include <algorithm>
#include <print>
#include <span>
#include <vector>

constexpr int combo(unsigned operand, Registers const &regs) {
	if (operand <= 3)
		return operand;
	if (operand == 4)
		return regs.A;
	if (operand == 5)
		return regs.B;
	if (operand == 6)
		return regs.C;

	return -1;
}

constexpr Registers process(Instruction inst, Registers r) {
	auto op = inst.operand;
	r.OUT = -1;
	switch (inst.opcode) {
			// clang-format off
		case OpCode::adv: { r.A = r.A >> combo(op, r);   r.SP++; } break;
		case OpCode::bxl: { r.B = r.B ^ op;              r.SP++; } break;
		case OpCode::bst: { r.B = combo(op, r) & 0b111;  r.SP++; } break;
		case OpCode::jnz: { if (r.A) r.SP = op / 2; else r.SP++; } break;
		case OpCode::bxc: { r.B = r.B ^ r.C;             r.SP++; } break;
		case OpCode::out: { r.OUT = combo(op,r) & 0b111; r.SP++; } break;
		case OpCode::bdv: { r.B = r.A >> combo(op, r);   r.SP++; } break;
		case OpCode::cdv: { r.C = r.A >> combo(op, r);   r.SP++; } break;
			// clang-format on
	}
	return r;
}

constexpr std::vector<int> run(std::span<const Instruction> prog, Registers init_regs) {
	std::vector<int> output;

	Registers regs = init_regs;

	while (regs.SP < prog.size()) {
		regs = process(prog[regs.SP], regs);
		if (regs.OUT >= 0) {
			output.push_back(regs.OUT & 0b111);
		}
	}

	return output;
}

template<size_t N>
constexpr std::array<Instruction, N / 2> parse(std::array<int, N> raw_prog) {
	std::array<Instruction, N / 2> prog;
	for (auto i = 0u; i < raw_prog.size(); i += 2) {
		prog[i / 2] = {static_cast<OpCode>(raw_prog[i]), raw_prog[i + 1]};
	}
	return prog;
}

enum class Result { NoMatch, Found, Solved };
constexpr Result try_bits(long testA, std::span<const Instruction> prog, std::span<const int> raw_prog) {
	auto out = run(prog, {.A = testA});

	if (std::equal(out.rbegin(), out.rend(), raw_program.rbegin())) {
		if (out.size() == raw_program.size())
			return Result::Solved;

		return Result::Found;
	}
	return Result::NoMatch;
}

constexpr long find_minimum_A(std::span<const Instruction> prog) {
	std::vector<long> solutions;
	std::vector<long> As;
	As.push_back(0);

	while (!As.empty()) {
		auto A = As.back();
		As.pop_back();

		for (unsigned bits = 0; bits < 8; bits++) {
			auto testA = A << 3 | bits;

			auto result = try_bits(testA, prog, raw_program);

			if (result == Result::Solved) {
				solutions.push_back(testA);
			} else if (result == Result::Found) {
				As.push_back(testA);
			}
		}
	}

	return *std::min_element(solutions.begin(), solutions.end());
}

int main(int argc, char *argv[]) {
	// Part 1:
	static_assert(run(parse(raw_program), initial_regs) == std::vector{7, 1, 5, 2, 4, 0, 7, 6, 1});
	std::println("Part 1:\n{}", run(parse(raw_program), initial_regs));

	// Part 2: BFS
	// Need to set -fconstexpr-steps > 10 million for clang, or something similar for gcc
	static_assert(find_minimum_A(parse(raw_program)) == 37222273957364);
	std::println("Part 2: Minimum value for A = {}", find_minimum_A(parse(raw_program)));

	return 0;
}

/// Tests:

static_assert(process({OpCode::adv, 3}, {.A = 22}).A == 2);
static_assert(process({OpCode::bxl, 7}, {.B = 29}).B == 26);
static_assert(process({OpCode::bst, 6}, {.C = 9}).B == 1);
static_assert(process({OpCode::bxc, 7}, {.B = 2024, .C = 43690}).B == 44354);

constexpr auto raw_test1 = std::array<int, 6>{0, 1, 5, 4, 3, 0};
constexpr auto test1 = std::array<Instruction, 3>{{{OpCode::adv, 1}, {OpCode::out, 4}, {OpCode::jnz, 0}}};
static_assert(parse(raw_test1) == test1);
static_assert(run(test1, {.A = 2024}) == std::vector{4, 2, 5, 6, 7, 7, 7, 7, 3, 1, 0});

constexpr auto raw_test2 = std::array<int, 6>{0, 1, 5, 4, 3, 0};
static_assert(run(parse(raw_test2), {.A = 729}) == std::vector{4, 6, 3, 5, 6, 3, 5, 2, 1, 0});
