#pragma once

struct Vec {
	long x;
	long y;

	constexpr bool operator==(Vec const &other) const {
		return x == other.x && y == other.y;
	}

	constexpr Vec operator/(Vec const &other) const {
		return {x / other.x, y / other.y};
	}

	constexpr Vec operator*(Vec const &other) const {
		return {x * other.x, y * other.y};
	}
};

struct Input {
	Vec A;
	Vec B;
	Vec prize;
};

struct RatVec {
	Vec num;
	Vec den;
};
