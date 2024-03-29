#include <iostream>

const int LIM = 1e4;

int main(int argc, char** argv) {
	srand(atoi(argv[1]));
	int n = rand()%LIM+1, q = rand()%LIM+1;
	std::cout << n << " " << q << std::endl;
	for (int i = 0; i < n; i++) std::cout << rand()%LIM << " ";
	std::cout << std::endl;
	while (q--) {
		int t = rand()%10;
		if (t == 0) {
			std::cout << "? " << rand()%n << std::endl;
			continue;
		} 
		if (t == 1) std::cout << "< ";
		if (t == 2) std::cout << "> ";
		if (t == 3) std::cout << "~ ";
		if (t == 4) std::cout << "$ ";
		if (t == 5) {
			std::cout << "= " << rand()%n << " ";
			std::cout << rand()%LIM << std::endl;
			continue;
		}
		if (t == 6) {
			std::cout << "p " << rand()%LIM << std::endl;
			continue;
		}
		if (t == 7) {
			std::cout << "P " << rand()%LIM << std::endl;
			continue;
		}
		if (t == 8) {
			std::cout << "/ " << rand()%LIM << std::endl;
			continue;
		}
		if (t == 9) {
			std::cout << "| " << rand()%(n+1) << std::endl;
			continue;
		}
		int l = rand()%n, r = rand()%n;
		if (l > r) std::swap(l, r);
		std::cout << l << " " << r << std::endl;
	}
	exit(0);
}

