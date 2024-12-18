#pragma once

struct Registers {
	long A{};
	long B{};
	long C{};
	long SP{};
	int OUT{};
};

enum class OpCode {
	adv = 0,
	bxl = 1,
	bst = 2,
	jnz = 3,
	bxc = 4,
	out = 5,
	bdv = 6,
	cdv = 7,
};

struct Instruction {
	OpCode opcode;
	int operand;

	constexpr bool operator==(Instruction const &other) const = default;
};
