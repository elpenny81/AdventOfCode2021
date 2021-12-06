#include <iostream>
#include <fstream>
#include <string>

namespace {

    int part1(const char* filename)
    {
        std::string direction;
        int steps;
        int x = 0, y = 0;

        std::ifstream input(filename);
        while (input >> direction >> steps) {
            if (direction == "down")
                y += steps;
            else if (direction == "up")
                y -= steps;
            else if (direction == "forward")
                x += steps;
        }

        return x * y;
    }

    int part2(const char* filename)
    {
        std::string direction;
        int steps;
        int x = 0, y = 0, aim = 0;

        std::ifstream input(filename);
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

        return x * y;
    }

};

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc02_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc02.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc02_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc02.txt") << std::endl;
}