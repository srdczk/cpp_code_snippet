#include <iostream>
#include <sstream>

// 字符串可作为流

int main() {
	std::string s("1 2 3");
	std::istringstream is(s);
	int n;
	while (is) {
		is >> n;
		if (is) std::cout << "Success:" << n << "\n";
		else std::cout << "Fail:" << "\n";
	}
	s = "1,2,3";
	char c;
	is = std::istringstream(s);
	while (is >> n >> c) {
		std::cout << "Success:" << n << " " << c << "\n";
	}
	return 0;
}