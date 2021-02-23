#include <iostream>
#include <chrono>
#include <vector>

#include "../../src/sms_mo.cpp"

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
	int k;
	std::cin >> k;

	k = 1<<k;

	std::vector<int> w = {0};
	for (int x = k/2; x; x /= 2) {
		int sz = w.size();
		for (int i = 0; i < sz; i++) w.push_back(w[i] + x);
	}

	std::vector<sms_mo<int>> v(k, sms_mo<int>(k*k-1));

	reset_time();

	for (int i = 0; i < k; i++) for (int j = 0; j < k; j++) v[i].insert(k*i + j);

	for (int it = 0; it < k; it++) {
		for (int d = 1; d < k; d *= 2)
			for (int i = 0; i + d < k; i += 2*d)
				v[w[i]].merge(v[w[i + d]]);

		swap(v[0], v[k-1]);
		for (int i = 0; i < k-1; i++) v[k-1].split_val(k*i + k, v[i]);
	}

	double time = get_time();

	std::cout << "sms_mo" << std::endl;
	std::cout << int(1000*time) << std::endl;

	return 0;
}
