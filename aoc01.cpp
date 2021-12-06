#include <iostream>
#include <fstream>
#include <optional>
#include <list>
#include <numeric>

namespace {

    int part1(const char* filename)
    {
        std::ifstream in(filename);
        std::optional<int> last_n;
        int n, n_increases = 0;
        while (in >> n) {
            if (last_n.has_value() && n > last_n)
                n_increases++;
            last_n = n;
        }
        return n_increases;
    }

    int part2(const char* filename)
    {
        std::ifstream in(filename);
        std::optional<int> last_n;
        int n, n_increases = 0, sum = 0;
        std::list<int> window;
        while (in >> n) {
            window.push_back(n);
            if (window.size() == 3) {
                sum = std::accumulate(window.begin(), window.end(), 0);
                window.pop_front();
                if (last_n.has_value() && sum > last_n)
                    n_increases++;
                last_n = sum;
            }
        }
        return n_increases;
    }

};

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc01_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc01.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc01_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc01.txt") << std::endl;
}