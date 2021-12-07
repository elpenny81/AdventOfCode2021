#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

namespace {

    int median(std::vector<int>& numbers)
    {
        std::vector<int>::iterator b = numbers.begin();
        std::vector<int>::iterator e = numbers.end();
        std::vector<int>::iterator med = b;

        std::advance(med, numbers.size() / 2);
        std::nth_element(b, med, e);
        return *med;
    }

    double mean(std::vector<int>& numbers)
    {
        return static_cast<double>(std::accumulate(numbers.begin(), numbers.end(), 0.0)) 
            / std::distance(numbers.begin(), numbers.end());
    }

    int part1(const char* filename)
    {
        std::ifstream in(filename);
        std::string token;
        std::vector<int> numbers;
        while (std::getline(in, token, ','))
            numbers.push_back(strtol(token.c_str(), nullptr, 0));

        const int med = median(numbers);
        int sum = 0;
        for (const auto &n : numbers)
            sum += std::abs(n - med);

        return sum;
    }

    int part2(const char* filename)
    {
        std::ifstream in(filename);
        std::string token;
        std::vector<int> numbers;
        while (std::getline(in, token, ','))
            numbers.push_back(strtol(token.c_str(), nullptr, 0));

        auto calc = [](std::vector<int>& numbers, bool ceil) -> int {
            int sum = 0;
            const double mn = ceil ? std::ceil(mean(numbers)) : std::floor(mean(numbers));
            for (const auto &n : numbers)
                sum += static_cast<int>(0.5 * std::abs(n - mn) * (std::abs(n - mn) + 1));
            return sum;
        };

        return std::min(calc(numbers, false), calc(numbers, true));
    }

}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc07_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc07.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc07_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc07.txt") << std::endl;
}