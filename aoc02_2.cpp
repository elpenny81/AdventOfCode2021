#include <iostream>
#include <fstream>
#include <string>

int main()
{
	std::string direction;
	int steps;
	int x = 0, y = 0, aim = 0;

	std::ifstream input("aoc02.txt");
	while (input >> direction >> steps) {
		if (direction == "down")
			aim += steps;
		else if (direction == "up")
			aim -= steps;
		else if (direction == "forward") {
			x += steps;
			y += steps * aim;
		}
	}

	std::cout << "Result = " << (x * y) << std::endl;
}