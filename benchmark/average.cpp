#include <iostream>
#include <string>

int main() {
	std::string s;
	int s1 = 0, s2 = 0;
	for (int i = 0; i < 10; i++) {
		std::cin.ignore();
		std::getline(std::cin, s);
		int a, x;
		std::cin >> x >> a;
		s1 += a;
		s2 += x;
	}
	std::cout << s << std::endl;
	//std::cout << s2 << std::endl;
	std::cout << s1/10 << " ms " << std::endl << std::endl;
	return 0;
}

