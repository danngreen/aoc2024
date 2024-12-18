#include "input.hh"
#include "regs.hh"
#include <print>
#include <span>

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
	// std::print("[{}]: {},{} => A:{:b} B:{:b} C:{:b}", r.SP, (int)inst.opcode, inst.operand, r.A, r.B, r.C);
	return r;
}

constexpr uint64_t run(std::span<const Instruction> prog, Registers init_regs) {
	uint64_t output{0};

	Registers regs = init_regs;

	while (regs.SP < prog.size()) {
		regs = process(prog[regs.SP], regs);
		if (regs.OUT >= 0) {
			output = (output << 4) | (regs.OUT & 0b111);
			// std::print(" => Out: {}", regs.OUT & 0b111);
		}
		// std::println("");
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

int main(int argc, char *argv[]) {
	// Part 1:
	static_assert(run(parse(raw_program), initial_regs) == 0x715240761);
	std::println("{:x}", run(parse(raw_program), initial_regs));

	// Part 2: In progress
	auto prog = parse(raw_program);
	auto output2 = run(prog, {.A = 0b001000000000000000000000000000000000000000000000});
	std::println("0x{:x}", output2);
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
static_assert(run(test1, {.A = 2024}) == 0x42567777310);

constexpr auto raw_test2 = std::array<int, 6>{0, 1, 5, 4, 3, 0};
static_assert(run(parse(raw_test2), {.A = 729}) == 0x4635635210);
