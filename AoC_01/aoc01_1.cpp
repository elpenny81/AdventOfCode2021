#include <iostream>
#include <fstream>

int main()
{
	std::ifstream input("input.txt");
	int n, last_n = -1, num_increases = 0;
	while (input >> n) {
		if (last_n != -1 && n > last_n)
			num_increases++;
		last_n = n;
	}

	std::cout << "Result = " << num_increases << std::endl;
}