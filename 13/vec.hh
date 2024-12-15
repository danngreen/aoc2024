#pragma once

struct Vec {
	long x;
	long y;

	bool operator==(Vec const &other) const {
		return x == other.x && y == other.y;
	}

	Vec operator/(Vec const &other) const {
		return {x / other.x, y / other.y};
	}

	Vec operator*(Vec const &other) const {
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
