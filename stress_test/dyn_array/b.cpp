#include <iostream>
#include <vector>
#include <algorithm>

int main() {
	int n, q;
	std::cin >> n >> q;
	std::vector<int> v;
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
			std::sort(v.begin()+l, v.begin()+r+1);
		} else if (c == '>') {
			int l, r;
			std::cin >> l >> r;
			sort(v.begin()+l, v.begin()+r+1);
			std::reverse(v.begin()+l, v.begin()+r+1);
		} else if (c == '~') {
			int l, r;
			std::cin >> l >> r;
			std::reverse(v.begin()+l, v.begin()+r+1);
		} else if (c == '$') {
			int l, r;
			std::cin >> l >> r;
			std::cout << *std::min_element(v.begin()+l, v.begin()+r+1) << std::endl;
		} else if (c == '=') {
			int idx, x;
			std::cin >> idx >> x;
			v[idx] = x;
		}
	}
	return 0;
}
