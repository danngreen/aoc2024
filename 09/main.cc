#include <fstream>
#include <iostream>
// #include <map>
// #include <set>
// #include <span>
// #include <vector>
#include <numeric>
#include <string>

void print_blocks(std::vector<int> const &blocks);

std::vector<int> parse_to_blocks(std::string filename) {
	std::ifstream ifs(std::string{filename});
	std::string line(std::istreambuf_iterator<char>{ifs}, {});
	line = line.substr(0, line.length() - 1); //trim ending newline
	// std::cout << line << "|\n";

	std::vector<int> blocks;
	// blocks.reserve(?);
	unsigned fileid = 0;
	bool isfile = true;
	for (auto c : line) {
		size_t num = c - '0';
		if (isfile) {
			blocks.insert(blocks.end(), num, fileid);
			fileid++;
		} else {
			blocks.insert(blocks.end(), num, -1);
		}
		isfile = !isfile;
	}

	return blocks;
}

// find_last_not(std::vector<int> &blocks) {
// }

void compact(std::vector<int> &blocks) {
	size_t end = 0;
	size_t beg = blocks.size();

	auto it_free = blocks.begin();
	auto it_file = blocks.rbegin();

	while (true) {
		auto next_free = std::find(it_free, blocks.end(), -1);
		auto next_file = std::find_if(it_file, blocks.rend(), [](auto &el) { return el >= 0; });

		auto next_free_idx = std::distance(blocks.begin(), next_free);
		auto next_file_idx = -std::distance(blocks.rend(), next_file) - 1; //??? -1 ??

		if (next_free_idx >= next_file_idx) {
			std::cout << "done: " << next_free_idx << " >= " << next_file_idx << "\n";
			return;
		}

		// std::cout << "[" << next_free_idx << "]<->[" << next_file_idx << "]" << "\n";
		// std::swap(next_free, next_file);
		// std::swap(blocks[next_free_idx], blocks[next_file_idx]);
		auto t = blocks[next_free_idx];
		blocks[next_free_idx] = blocks[next_file_idx];
		blocks[next_file_idx] = t;

		it_free = next_free + 1;
		it_file = next_file + 1;
		// print_blocks(blocks);
	}
}

long long checksum(std::vector<int> &blocks) {
	int idx = -1;
	return std::accumulate(blocks.begin(), blocks.end(), 0LL, [&idx](long long sum, int el) {
		idx++;
		if (el >= 0) {
			return sum + el * idx;
		} else {
			return sum;
		};
	});
}

int main(int argc, char *argv[]) {
	auto blocks = parse_to_blocks(argv[1]);
	// print_blocks(blocks);
	compact(blocks);
	// print_blocks(blocks);

	auto chk = checksum(blocks);

	std::cout << "Part 1: " << chk << "\n";
	// std::cout << "Part 2: " << antinodes.size() << "\n";
	return 0;
}

void print_blocks(std::vector<int> const &blocks) {

	for (auto e : blocks) {
		// std::cout << std::setfill('0') << std::setw(4);

		if (e == -1)
			std::cout << "."; //"____ ";
		else
			std::cout << e << "";
	}
	std::cout << "\n";
}
