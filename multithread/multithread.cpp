// shitty thread example

#include <iostream>
#include <thread>
#include <random>
#include <time.h>

void f(int x) {
	std::cout << "Thread " << x << ": hi\n";
}

int main() {
	std::cout << "Enter number of threads > ";
	std::string input;
	int threads;
	getline(std::cin, input);
	try {
		threads = stoi(input);
	}
	catch (...) {
		std::cout << "Enter a number";
		return 0;
	}
	if (threads < 1) {
		std::cout << "Enter a number larger than 0";
	} else {
		std::thread s[threads];
		for (int i=0;i<threads;i++) {
			s[i] = std::thread(f, i);
		}
		for (int i=0;i<threads;i++) {
			s[i].join();
		}
	}
	return 0;
}
