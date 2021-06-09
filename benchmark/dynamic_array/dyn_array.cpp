#include <iostream>
#include <chrono>
#include <vector>

#include "../../src/dyn_array.cpp"

std::chrono::time_point<std::chrono::high_resolution_clock> beg;

void reset_time() {
	beg = std::chrono::high_resolution_clock::now();
}

double get_time() {
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - beg;
	return elapsed.count();
}

int main(int argc, char** argv) {
	srand(atoi(argv[1]));

	int NUM;
	std::cin >> NUM;

	int n;
	std::cin >> n;

	int sum = 0;

	std::vector<dyn_array<int>> v(NUM);

	reset_time();

	for (int i = 0; i < n; i++) {
		int op = rand()%5;
		int x = rand()%NUM;
		if (op == 0) v[x].push_back(rand()%n);
		else if (op == 1) {
			int idx = rand()%n;
			v[x].update(idx, rand()%n);
		} else if (op == 2) {
			int x2 = rand()%NUM;
			dyn_array<int> tmp;
			v[x].split(rand()%(n+1), tmp);
			v[x2].concat(tmp);
		} else if (op == 3) v[x].sort();
		else if (op == 4) {
			int x2 = rand()%NUM;
			dyn_array<int> tmp;
			v[x].partition(rand()%(n+1), tmp);
			v[x2].concat(tmp);
		}
	}

	double time = get_time();

	std::cout << "dyn_array" << std::endl;
	std::cout << sum << std::endl;
	std::cout << int(1000*time) << std::endl;

	return 0;
}
