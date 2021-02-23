#include <iostream>
#include <chrono>
#include <vector>

#include "../../src/sms.cpp"

std::chrono::time_point<std::chrono::high_resolution_clock> beg;

void reset_time() {
	beg = std::chrono::high_resolution_clock::now();
}

double get_time() {
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - beg;
	return elapsed.count();
}

int main() {
	int n;
	std::cin >> n;

	std::vector<int> w = {0};
	if (n) for (int x = 1<<(n-1); x; x /= 2) {
		int sz = w.size();
		for (int i = 0; i < sz; i++) w.push_back(w[i] + x);
	}

	std::vector<sms<int>> v(1<<n, sms<int>((1<<n)-1));

	reset_time();

	for (int i = 0; i < (1<<n); i++) v[i].insert(w[i]);

	for (int d = 1; d < (1<<n); d *= 2)
		for (int i = 0; i + d < (1<<n); i += 2*d)
			v[i].merge(v[i + d]);

	double time = get_time();

	std::cout << "sms" << std::endl;
	std::cout << int(1000*time) << std::endl;

	return 0;
}
