#include <iostream>

const int LIM = 1e6;

int main(int argc, char** argv) {
	srand(atoi(argv[1]));
	int q = rand()%LIM+1;
	std::cout << q << std::endl;
	while (q--) {
		int t = rand()%4;
		if (t == 0) std::cout << "+ ";
		if (t == 1) std::cout << "- ";
		if (t == 2) std::cout << "? ";
		if (t == 3) {
			std::cout << "$" << std::endl;
			continue;
		}
		std::cout << rand()%LIM << std::endl;
	}
	exit(0);
}

