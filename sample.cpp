#include <iostream>

#include "src/sms.cpp"
#include "src/sms_mo.cpp"
#include "src/dyn_array.cpp"

int main() {
	sms<int> s = {1, 3, 5, 8};
	for (int i : s.to_vector()) std::cout << i << " ";
	std::cout << std::endl;

	sms_mo<int> s_mo = {1, 3, 5, 8};
	for (int i : s_mo.to_vector()) std::cout << i << " ";
	std::cout << std::endl;

	dyn_array<int> v = {1, 3, 5, 8};
	for (int i : v.to_vector()) std::cout << i << " ";
	std::cout << std::endl;

	return 0;
}
