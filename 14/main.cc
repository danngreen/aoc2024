#include "../common/fileparse.hh"
#include <iostream>
#include <span>

constexpr int MAXX = 101;
constexpr int MAXY = 103;

// for tests:
// constexpr int MAXX = 11;
// constexpr int MAXY = 7;

struct Vec {
	int x{};
	int y{};

	constexpr Vec operator+(Vec const &other) const {
		return wrap({x + other.x, y + other.y});
	}

	static constexpr Vec wrap(Vec other) {
		Vec v = other;
		while (v.x < 0)
			v.x += MAXX;
		while (v.x >= MAXX)
			v.x = v.x - MAXX;
		while (v.y < 0)
			v.y += MAXY;
		while (v.y >= MAXY)
			v.y = v.y - MAXY;
		return v;
	}

	constexpr Vec operator*(int m) const {
		return {x * m, y * m};
	}
};

constexpr Vec MAX{MAXX, MAXY};

struct Bot {
	Vec p{};
	Vec v{};

	void step() {
		p = (p + v);
	}

	void step(int steps) {
		p = (p + (v * steps));
	}
};

std::vector<Bot> parse(std::string_view filename) {
	std::vector<Bot> bots;

	auto lines = parse_lines(filename);
	for (auto line : lines) {
		Bot b;
		auto cpos = line.find(',');
		b.p.x = std::stoi(line.substr(2, cpos));

		auto spos = line.find(' ', cpos);
		b.p.y = std::stoi(line.substr(cpos + 1, spos));

		auto vcpos = line.find(',', spos);
		b.v.x = std::stoi(line.substr(spos + 3, vcpos));

		auto vspos = line.find(' ', vcpos);
		b.v.y = std::stoi(line.substr(vcpos + 1, vspos));

		bots.push_back(b);
	}

	return bots;
}

void print(std::span<Bot> bots);

unsigned calc_safety(std::span<Bot> bots) {
	std::array<unsigned, 4> quads{};

	for (auto &bot : bots) {
		if (bot.p.x < MAXX / 2 && bot.p.y < MAXY / 2)
			quads[0]++;

		if (bot.p.x > MAXX / 2 && bot.p.y < MAXY / 2)
			quads[1]++;

		if (bot.p.x > MAXX / 2 && bot.p.y > MAXY / 2)
			quads[2]++;

		if (bot.p.x < MAXX / 2 && bot.p.y > MAXY / 2)
			quads[3]++;
	}

	return quads[0] * quads[1] * quads[2] * quads[3];
}

int main(int argc, char *argv[]) {

	auto bots = parse(argv[1]);

	auto bots2 = bots;

	for (auto &bot : bots) {
		bot.step(100);
	}

	std::cout << "Part 1: " << calc_safety(bots) << "\n";

	for (unsigned i = 0; i < 10000; i++) {
		for (auto &bot : bots2) {
			bot.step();
		}

		if (i == 7501) // determined by searching for "1111111111111111111" in output
			print(bots2);
		//TODO: generate a map on each step and search for 111111111111111111 in any line
	}

	std::cout << "Part 2: " << 7502 << "\n";

	return 0;
}

void print(std::span<Bot> bots) {
	std::array<std::array<int, MAXX>, MAXY> map{};

	for (auto &bot : bots) {
		map[bot.p.y][bot.p.x]++;
	}

	for (auto &line : map) {
		for (auto p : line) {
			if (p == 0)
				std::cout << ".";
			else
				std::cout << p;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}
