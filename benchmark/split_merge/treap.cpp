#include <iostream>
#include <chrono>
#include <vector>

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

const int NUM = 100;

int main(int argc, char** argv) {
	srand(atoi(argv[1]));

	long long MAX;
	std::cin >> MAX;
	int n;
	std::cin >> n;

	int sum = 0;

	std::vector<treap<long long>> v(NUM);

	reset_time();

	for (int i = 0; i < n; i++) {
		if (rand() % NUM) {
			int op = rand()%3;
			int idx = rand()%NUM;
			long long a = (rand()*(long long)rand())%MAX;
			if (op == 0) v[idx].insert(a);
			else if (op == 1) v[idx].erase(a);
			else sum += v[idx].count(a);
		} else {
			int a = rand()%NUM, b = rand()%NUM;
			int qt = rand()%(v[a].size()+1);
			treap<long long> tmp;
			v[a].split(qt, tmp);
			v[b].merge(tmp);
		}
	}

	double time = get_time();

	std::cout << "treap" << std::endl;
	std::cout << sum << std::endl;
	std::cout << int(1000*time) << std::endl;

	return 0;
}
