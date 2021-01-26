#include <iostream>

#include "src/sms.cpp"
#include "src/dyn_array.cpp"

int main() {
	sms<int> s = {1, 3, 5, 8};
	for (int i = 0; i < s.size(); i++)
		std::cout << s[i] << " ";
	std::cout << std::endl;

	dyn_array<int> v = {1, 3, 5, 8};
	for (int i = 0; i < s.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;

	return 0;
}
