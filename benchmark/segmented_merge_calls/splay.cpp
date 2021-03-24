#include <iostream>
#include <vector>

#include "../../src/benchmark/splay.cpp"

int main() {
	int k;
	std::cin >> k;

	k = 1<<k;

	std::vector<int> w = {0};
	for (int x = k/2; x; x /= 2) {
		int sz = w.size();
		for (int i = 0; i < sz; i++) w.push_back(w[i] + x);
	}

	std::vector<splaytree<int>> v(k);

	for (int i = 0; i < k; i++) for (int j = 0; j < k; j++) v[i].insert(k*i + j);

	for (int i = 0; i < k; i++) v[i].calls = 0;
	int op = 0;

	for (int it = 0; it < k; it++) {
		for (int d = 1; d < k; d *= 2)
			for (int i = 0; i + d < k; i += 2*d)
				v[w[i]].merge(v[w[i + d]]), op++;

		swap(v[0], v[k-1]);
		for (int i = 0; i < k-1; i++) v[k-1].split(k*i + k, v[i]);
	}

	int calls = 0;
	for (int i = 0; i < k; i++) calls += v[i].calls;

	std::cout << "splay" << std::endl;
	std::cout << calls/op << std::endl;

	return 0;
}
