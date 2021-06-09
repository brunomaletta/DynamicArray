#include <iostream>

#include "../../src/sms.cpp"

int main() {
	int q;
	std::cin >> q;
	sms<int> s;
	while (q--) {
		char c;
		std::cin >> c;
		if (c == '+') {
			int x;
			std::cin >> x;
			s.insert(x);
		} else if (c == '-') {
			int x;
			std::cin >> x;
			s.erase(x);
		} else if (c == '?') {
			int x;
			std::cin >> x;
			if (s.count(x)) std::cout << 1 << std::endl;
			else std::cout << 0 << std::endl;
		} else if (c == '$') {
			if (!s.size()) std::cout << -1 << std::endl;
			else std::cout << s.min() << " " << s.max() << std::endl;
		}
	}
	return 0;
}
