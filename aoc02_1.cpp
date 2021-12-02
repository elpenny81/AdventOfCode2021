#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string direction;
    int steps;
    int x = 0, y = 0;

    std::ifstream input("aoc02.txt");
    while (input >> direction >> steps) {
        if (direction == "down")
            y += steps;
        else if (direction == "up")
            y -= steps;
        else if (direction == "forward")
            x += steps;
    }

    std::cout << "Result = " << (x * y) << std::endl;
}