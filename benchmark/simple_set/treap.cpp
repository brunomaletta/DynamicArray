#include <iostream>
#include <chrono>

#include "../../src/bst/treap.cpp"

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

	long long MAX;
	std::cin >> MAX;
	int n;
	std::cin >> n;

	int sum = 0;

	treap<long long> s;

	reset_time();

	for (int i = 0; i < n; i++) {
		int op = rand()%3;
		long long a = (rand()*(long long)rand())%MAX;
		if (op == 0) s.insert(a);
		else if (op == 1) s.erase(a);
		else sum += s.count(a);
	}

	double time = get_time();

	std::cout << "treap" << std::endl;
	std::cout << sum << std::endl;
	std::cout << int(1000*time) << std::endl;

	return 0;
}
