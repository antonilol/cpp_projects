#include <iostream>
#include <time.h>
#include "PerlinNoise/PerlinNoise.hpp"

#define UPPER_HALF_BLOCK "\xe2\x96\x80"

#define W 80
#define H 80

#define S 50

#define C_RGB_FG "\x1b[38;2;"
#define C_RGB_BG "\x1b[48;2;"

#define C_RESET "\x1b[0m"

#define SEED_TEXT "Using srand seed %u (time(NULL))"

using namespace std;

int main() {
	const unsigned int srandSeed = time(NULL);
	srand(srandSeed);
	fprintf(stderr, SEED_TEXT "\n", srandSeed);

	const siv::PerlinNoise perlinR(rand());
	const siv::PerlinNoise perlinG(rand());
	const siv::PerlinNoise perlinB(rand());

	for (double i = 0; i < H/2; i++) {
		for (double j = 0; j < W; j++) {

			const double x  =  j          / S;
			const double y1 =  i * 2      / S;
			const double y2 = (i * 2 + 1) / S;

			const unsigned int r1 = perlinR.accumulatedOctaveNoise2D_0_1(x, y1, 8) * 255;
			const unsigned int g1 = perlinG.accumulatedOctaveNoise2D_0_1(x, y1, 8) * 255;
			const unsigned int b1 = perlinB.accumulatedOctaveNoise2D_0_1(x, y1, 8) * 255;
			const unsigned int r2 = perlinR.accumulatedOctaveNoise2D_0_1(x, y2, 8) * 255;
			const unsigned int g2 = perlinG.accumulatedOctaveNoise2D_0_1(x, y2, 8) * 255;
			const unsigned int b2 = perlinB.accumulatedOctaveNoise2D_0_1(x, y2, 8) * 255;

			cout << C_RGB_FG << r1 << ";" << g1 << ";" << b1 << "m" C_RGB_BG << r2 << ";" << g2 << ";" << b2 << "m" UPPER_HALF_BLOCK;
		}
		cout << C_RESET "\n";
	}
}
