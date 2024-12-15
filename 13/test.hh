#include "vec.hh"
#include <array>

constexpr std::array input = {
	Input{{94, 34}, {22, 67}, {8400, 5400}},
	Input{{26, 66}, {67, 21}, {12748, 12176}},
	Input{{17, 86}, {84, 37}, {7870, 6450}},
	Input{{69, 23}, {27, 71}, {18641, 10279}},
};

// Parse input:
// %s/Button A: X+\(\d\+\), Y+\(\d\+\)/Input{{\1, \2}, /
// %s/Button B: X+\(\d\+\), Y+\(\d\+\)/{\1, \2}, /
// %s/Prize: X=\(\d\+\), Y=\(\d\+\)/{\1, \2}}, /
