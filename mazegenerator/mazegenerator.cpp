#include <iostream>
#include <climits>

#define FULL_BLOCK "██"

#define MUST_BE " must be an odd number larger than 2\n"
#define INVAL_ARG "Invalid argument\n"
#define USAGE1 "Usage: "
#define USAGE2 \
	" <w> <h> [start_seed=0] [style=0]\n" \
	"style: 0=classic 1=solid 2=unicode 3=unicode bold\n"

// 0 = empty maze, 256 = full and never finds a path
#define FILL 140

// start pos = left bottom corner
#define SX 0
#define SY h - 1
// end pos = right top corner
#define EX w - 1
#define EY 0

bool *maze;
uint w, h, style;

const std::string box_draw =
	"\u00b7\u2022\u2574\u2578"
	"\u2577\u257b\u2510\u2513"
	"\u2576\u257a\u2500\u2501"
	"\u250c\u250f\u252c\u2533"
	"\u2575\u2579\u2518\u251b"
	"\u2502\u2503\u2524\u252b"
	"\u2514\u2517\u2534\u253b"
	"\u251c\u2523\u253c\u254b";

std::string get_char(uint i) {
	if (i) {
		return box_draw.substr(i * 3 - 1, 3);
	}
	return box_draw.substr(0, 2);
}

void generate(uint seed) {
	srand(seed);
	for (uint y = 0; y < h; y++) {
		for (uint x = 0; x < w; x++) {
			maze[y * w + x] =
				(x & 1 && y & 1) ||
				((y + x) & 1 && (rand() & 0xff) < FILL);
		}
	}
}

bool check(const uint x, const uint y) {
	if (x == EX && y == EY) {
		return 1;
	}

	if (x >= w || y >= h || maze[y * w + x]) {
		return 0;
	}

	maze[y * w + x] = 1;
	return
		check(x - 1, y    ) ||
		check(x    , y + 1) ||
		check(x + 1, y    ) ||
		check(x,     y - 1);
}

void print_maze() {
	if (style == 0) {
		std::cout << '+' << std::string(w * 2, '-') << '+';
	} else if (style == 1) {
		for (uint i = 0; i < w + 2; i++) {
			std::cout << FULL_BLOCK;
		}
	} else if (style < 4) {
		std::cout << get_char(0b01100 | style == 3);
		for (uint x = 0; x < w; x++) {
			for (uint i = 0; i < !(x & 1) + 1; i++) { // TODO optimize
				std::cout << get_char(0b01010 | (maze[x]) << 2 | style == 3);
			}
		}
		std::cout << get_char(0b00110 | style == 3);
	}

	for (uint y = 0; y < h; y++) {
		std::cout << "\n";
		if (y + 1 == h) {
			std::cout << (style == 1 ? "  " : " ");
		} else {
			if (style == 0) {
				std::cout << "|";
			} else if (style == 1) {
				std::cout << FULL_BLOCK;
			} else if (style < 4) {
				std::cout << get_char(0b10100 | (maze[y * w]) << 3 | (style == 3));
			}
		}

		for (uint x = 0; x < w; x++) {
			if (maze[y * w + x]) {
				if (style < 2) {
					std::cout << (style == 0 ? "##" : FULL_BLOCK);
				} else if (style < 4) {
					for (uint i = 0; i < !(x & 1) + 1; i++) { // TODO optimize
						std::cout << get_char(
							(y     == 0 || maze[y * w + x - w]) << 4 |
							(x + 1 == w || maze[y * w + x + 1]) << 3 |
							(y + 1 == h || maze[y * w + x + w]) << 2 |
							(x     == 0 || maze[y * w + x - 1]) << 1 |
							 style == 3
						);
					}
				}
			} else {
				std::cout << (style < 2 || !(x & 1) ? "  " : " ");
			}
		}

		if (y == 0) {
			std::cout << (style == 1 ? "  " : " ");
		} else {
			if (style == 0) {
				std::cout << '|';
			} else if (style == 1) {
				std::cout << FULL_BLOCK;
			} else if (style < 4) {
				std::cout << get_char(0b10100 | (maze[(y + 1) * w - 1]) << 1 | (style == 3));
			}
		}
	}

	std::cout << "\n";
	if (style == 0) {
		std::cout << '+' << std::string(w * 2, '-') << "+\n";
	} else if (style == 1) {
		std::cout << FULL_BLOCK FULL_BLOCK;
		for (uint i = 0; i < w; i++) {
			std::cout << FULL_BLOCK;
		}
	} else if (style < 4) {
		std::cout << get_char(0b11000 | style == 3);
		for (uint x = 0; x < w; x++) {
			for (uint i = 0; i < !(x & 1) + 1; i++) { // TODO optimize
				std::cout << get_char(0b01010 | (maze[(h - 1) * w + x]) << 4 | style == 3);
			}
		}
		std::cout << get_char(0b10010 | style == 3);
	}
}

int parse_uint(char *s, uint &ui) {
	try {
		long int li = std::stol(s);
		if (li < 0 || li > UINT_MAX) {
			return 1;
		}
		ui = (uint) li;
		return 0;
	}
	catch (...) {
		return 1;
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cout << USAGE1 << argv[0] << USAGE2;
		return 1;
	}

	if (
		parse_uint(argv[1], w) ||
		parse_uint(argv[2], h)
	) {
		std::cout << INVAL_ARG;
		return 1;
	}
	if (w < 3 || !(w & 1)) {
		std::cout << "Width" MUST_BE;
		return 1;
	}
	if (h < 3 || !(h & 1)) {
		std::cout << "Height" MUST_BE;
		return 1;
	}

	uint seed = 0;

	if (argc > 3) {
		if (
			parse_uint(argv[3], seed)
		) {
			std::cout << INVAL_ARG;
			return 1;
		}
	}

	style = 0;

	if (argc > 4) {
		if (
			parse_uint(argv[4], style)
		) {
			std::cout << INVAL_ARG;
			return 1;
		}
		if (style > 3) {
			std::cout << INVAL_ARG;
			return 1;
		}
	}

	const uint start_seed = seed;
	bool m[h * w];
	maze = m;

	std::cout << "\nWorking...\n";

	while (1) {
		generate(seed);

		if (check(SX, SY)) {
			break;
		}

		seed++;
		if (seed == start_seed) {
			std::cout << "Nothing found :(\n";
			return 0;
		}
	}

	std::cout << "\nseed = " << seed << "\n\n";
	generate(seed);
	print_maze();

	return 0;
}
