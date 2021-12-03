#include <iostream>
#include <stdlib.h>
#include <time.h>

#define W 7
#define H 6
#define ROW 4

#define C_RESET "\33[0m"
#define BALL "\xe2\x97\x8f"
#define C_RED "\33[1;31m"
#define C_YELLOW "\33[1;33m"

#define RED_OR_YELLOW ( turnR ? C_RED "Red" : C_YELLOW "Yellow" ) << C_RESET

#define C1 "┌"
#define C2 "┐"
#define C3 "└"
#define C4 "┘"	
#define B1 "─"
#define B2 "│"

#define R 1
#define Y 2

using namespace std;

bool turnR;
uint8_t board[W*H];
uint16_t wins[2] = {0};

void drawBoard() {
	cout << "\n" C1;
	for (uint8_t i=0;i<W*2-1;i++) {
		cout << B1;
	}
	cout << C2 "\n";

	for (uint8_t i=0;i<H;i++) {
		cout << B2;
		for (uint8_t j=0;j<W;j++) {
			if (j) {
				cout << " ";
			}
			const uint8_t b = board[j+i*W];
			if (b == R) {
				cout << C_RED BALL C_RESET;
			} else if (b == Y) {
				cout << C_YELLOW BALL C_RESET;
			} else {
				cout << " ";
			}
		}
		cout << B2 "\n";
	}

	cout << C3;
	for (uint8_t i=0;i<W*2-1;i++) {
		cout << B1;
	}
	cout << C4 "\n";
	for (uint8_t i=0;i<W;i++) {
		cout << " " << (const char) ('A' + i);
	}
	cout << "\n\n";
}

void win() {
	drawBoard();
	wins[!turnR] += 1;
	cout << RED_OR_YELLOW " won!\n" C_RED << wins[0] << C_RESET "-" C_YELLOW << wins[1] << C_RESET "\n";
}

void play() {
	turnR = rand() & 1;
	cout << RED_OR_YELLOW " starts.\n";
	fill(board, board+W*H, 0);

	while (1) {
		const uint8_t P = turnR ? R : Y;
		drawBoard();

		// input
		uint8_t col;
		string p;
		while (1) {
			cout << RED_OR_YELLOW " enter A-" << (const char) ('A' + W - 1) << " > ";
			getline(cin, p);
			if (p.size() != 1) {
				if (p == "gg") {
					cout << RED_OR_YELLOW " gg'ed!";
					turnR = !turnR;
					win();
					return;
				}
				cout << "Input one letter\n";
			} else {
				const char c = p.at(0);
				if (c < 'A' || c > 'A' + W - 1) {
					cout << "Input one letter in the range A-" << (const char) ('A' + W - 1) << "\n";
				} else {
					col = c - 'A';
					if (board[col]) {
						cout << "That column is full!\n";
					} else {
						break;
					}
				}
			}
		}

		// gravity
		for (uint8_t i=H-1;i!=0xff;i--) {
			if (!board[col+W*i]) {
				board[col+W*i] = P;
				break;
			}
		}

		// win check
		bool w,x;
		for (uint8_t i=0;i<H;i++) {
			// horizontal
			for (uint8_t j=0;j<W-ROW+1;j++) {
				w = 1;
				for (uint8_t v=0;v<ROW;v++) {
					w = w && board[j+i*W+v] == P;
				}
				if (w) {
					win();
					return;
				}
			}
			if (i > H - ROW) {
				continue;
			}

			// vertical
			for (uint8_t j=0;j<W;j++) {
				w = 1;
				for (uint8_t v=0;v<ROW;v++) {
					w = w && board[j+(i+v)*W] == P;
				}
				if (w) {
					win();
					return;
				}

				if (j > W - ROW) {
					continue;
				}

				// diagonal
				w = 1;
				x = 1;
				for (uint8_t v=0;v<ROW;v++) {
					w = w && board[j+(i+v)*W+v      ] == P;
					x = x && board[j+(i+v)*W+ROW-v-1] == P;
				}
				if (w || x) {
					win();
					return;
				} 
			}
		}
		turnR = !turnR;
	}
}

int main() {
	srand(time(NULL));

	while (1) {
		play();
	}
}
