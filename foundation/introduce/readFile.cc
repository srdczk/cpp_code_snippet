#include <iostream>
#include <fstream>

void readFile(std::string path) {
	std::ifstream is;
	is.open(path, std::ios::in);
	std::string p;
	while (is >> p) std::cout << p << "\n";
}

int main() {
	readFile("readFile.cc");
	return 0;
}