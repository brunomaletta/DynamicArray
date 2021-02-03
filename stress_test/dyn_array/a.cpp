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
			std::cout << v.rmq(l, r) << std::endl;
		}
	}
	return 0;
}
