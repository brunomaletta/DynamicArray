#include <iostream>

#include "../../src/dyn_array.cpp"

int main() {
	int n, q;
	std::cin >> n >> q;
	dyn_array<int> v;
	for (int i = 0; i < n; i++) {
		int x;
		std::cin >> x;
		v.push_back(x);
	}
	while (q--) {
		char c;
		std::cin >> c;
		if (c == '?') {
			int idx;
			std::cin >> idx;
			std::cout << v[idx] << std::endl;
		} else if (c == '<') {
			int l, r;
			std::cin >> l >> r;
			v.sort(l, r);
		} else if (c == '>') {
			int l, r;
			std::cin >> l >> r;
			v.sort(l, r, true);
		} else if (c == '~') {
			int l, r;
			std::cin >> l >> r;
			v.reverse(l, r);
		} else if (c == '$') {
			int l, r;
			std::cin >> l >> r;
			std::cout << v.rmq(l, r) << " " << v.rmq(l, r, true) << std::endl;
		} else if (c == '=') {
			int idx, x;
			std::cin >> idx >> x;
			v.update(idx, x);
		} else if (c == 'p') {
			int x;
			std::cin >> x;
			std::cout << v.first_smaller(x) << std::endl;
		} else if (c == 'P') {
			int x;
			std::cin >> x;
			std::cout << v.first_not_smaller(x) << std::endl;
		} else if (c == '/') {
			int x;
			std::cin >> x;
			dyn_array<int> tmp;
			v.partition(x, tmp);
			tmp.concat(v);
			swap(v, tmp);
		} else if (c == '|') {
			int x;
			std::cin >> x;
			dyn_array<int> tmp;
			v.partition_k(x, tmp);
			tmp.concat(v);
			swap(v, tmp);
		}
	}
	return 0;
}
