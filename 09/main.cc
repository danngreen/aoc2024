#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

void print_blocks(std::vector<int> const &blocks);

struct Entry {
	int start;
	int size;
};

// filetab and spacetab are used for part 2
std::vector<Entry> filetab;
std::vector<Entry> spacetab;

// blocks is used for part 1

std::vector<int> parse_to_blocks(std::string filename) {
	std::ifstream ifs(std::string{filename});
	std::string line(std::istreambuf_iterator<char>{ifs}, {});
	line = line.substr(0, line.length() - 1); //trim ending newline

	std::vector<int> blocks;

	bool isfile = true;
	for (auto c : line) {
		size_t size = c - '0';

		if (isfile) {
			auto fileid = filetab.size();
			filetab.push_back(Entry(blocks.size(), size));
			blocks.insert(blocks.end(), size, fileid);
		} else {
			spacetab.push_back(Entry(blocks.size(), size));
			blocks.insert(blocks.end(), size, -1);
		}

		isfile = !isfile;
	}

	return blocks;
}

void compact(std::vector<int> &blocks) {
	auto next_free = blocks.begin();
	auto next_file = blocks.rbegin();

	while (true) {
		next_free = std::find_if(next_free, blocks.end(), [](auto &el) { return el < 0; });
		next_file = std::find_if(next_file, blocks.rend(), [](auto &el) { return el >= 0; });

		if (std::make_reverse_iterator(next_free) <= next_file)
			return;

		std::swap(*next_free, *next_file);

		next_free++;
		next_file++;

		// print_blocks(blocks);
	}
}

void compact_whole(std::vector<int> &blocks) {
	// try to move each file, starting with the last
	for (size_t fileid = filetab.size() - 1; fileid > 0; fileid--) {
		auto &file = filetab[fileid];

		auto free = std::find_if(spacetab.begin(), spacetab.end(), [&file](Entry const &e) {
			return e.size >= file.size && e.start < file.start;
		});

		if (free != spacetab.end()) {
			file.start = free->start;
			*free = {.start = free->start + file.size, .size = free->size - file.size};
		}
	}
}

long long checksum_blocks(std::vector<int> &blocks) {
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

long long checksum_filetab(std::vector<Entry> const &filetab) {
	long long sum = 0;

	for (unsigned fileid = 0u; auto file : filetab) {
		for (unsigned i = file.start; i < file.start + file.size; i++) {
			sum += i * fileid;
		}
		fileid++;
	}

	return sum;
}

int main(int argc, char *argv[]) {
	const auto blocks = parse_to_blocks(argv[1]);

	{
		auto blocks1 = blocks;

		compact(blocks1);

		auto chk = checksum_blocks(blocks1);

		std::cout << "Part 1: " << chk << "\n";
		// 6334655979668
	}

	{
		auto blocks2 = blocks;

		compact_whole(blocks2);

		auto chk2 = checksum_filetab(filetab);

		std::cout << "Part 2: " << chk2 << "\n";
		//   6349492251099
	}

	return 0;
}

void print_blocks(std::vector<int> const &blocks) {

	for (auto e : blocks) {

		if (e < 0)
			std::cout << e << " ";
		else
			std::cout << " " << e << " ";
	}
	std::cout << "\n";
}
