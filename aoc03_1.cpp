#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
    std::ifstream input("aoc03.txt");
    std::string line;
    std::vector<int> count;
    while (input >> line) {
        if (count.size() == 0)
            count.resize(line.length());

        for (std::size_t i = 0; i < line.length(); i++) {
            if (line.at(i) == '0')
                count[i]--;
            if (line.at(i) == '1')
                count[i]++;
        }
    }

    int gamma = 0, epsilon = 0;

    for (size_t i = 0; i < count.size(); i++) {
        gamma = gamma << 1 | (count.at(i) > 0);
        epsilon = epsilon << 1 | (count.at(i) < 0);
    }

    std::cout << "Result = " << (gamma * epsilon) << std::endl;
}
