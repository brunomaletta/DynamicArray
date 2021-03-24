#include <iostream>
#include <string>

int main(int argc, char** argv) {
	int CNT = atoi(argv[1]);
	std::string s;
	int s1 = 0, s2 = 0, mem = 0;
	for (int i = 0; i < CNT; i++) {
		std::cin.ignore();
		std::getline(std::cin, s);
		int a, m;
		std::cin >> a >> m;
		s1 += a;
		mem += m;
	}
	std::cout << s << std::endl;
	std::cout << s1/CNT << " calls / operation" << std::endl;
	std::cout << mem/CNT << " MB" << std::endl;
	return 0;
}

