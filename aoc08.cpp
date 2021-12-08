#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cstdint>

#include "toolbox.h"

namespace {
    int part1(const char* filename)
    {
        std::ifstream in(filename);
        std::string line;
        int result = 0;

        // unique segments number          1  7  4  8
        const std::vector<int> n_segment { 2, 3, 4, 7 };

        while (std::getline(in, line)) {
            auto parts = tools::string_split(line, " | ");
            auto output = tools::string_split(parts.at(1), " ");
            for (auto& e : output) {
                if (tools::contains(n_segment, e.size()))
                    result++;
            }
        }

        return result;
    }

    int part2(const char* filename)
    {
        std::ifstream in(filename);
        std::string line;
        int sum = 0;

        auto func_remove = [](std::string s1, std::string s2) -> std::string {
            s1.erase(std::remove_if(s1.begin(), s1.end(), [s2](const char& c) {
                return s2.find(c) != std::string::npos;
            }));
            return s1;
        };

        auto func_contain = [](std::string s1, std::string s2) -> bool {
            for (auto c : s2)
                if (s1.find(c) == std::string::npos)
                    return false;
            return true;
        };

        while (std::getline(in, line)) {
            auto parts = tools::string_split(line, " | ");
            auto input = tools::string_split(parts.at(0), " ");
            auto output = tools::string_split(parts.at(1), " ");

            std::map<int, char> segments;
            std::map<int, std::vector<std::string>> tmp;
            std::vector<std::string> numbers(10);
            for (auto& e : input) {
                std::sort(e.begin(), e.end());
                tmp[e.size()].push_back(e);
            }

            // this is 1
            numbers[1] = tmp[2].at(0);
            numbers[4] = tmp[4].at(0);
            numbers[7] = tmp[3].at(0);
            numbers[8] = tmp[7].at(0);

            segments[0] = func_remove(numbers[7], numbers[1]).at(0);
            
            for (auto &it = tmp[5].begin(); it != tmp[5].end(); it++) {
                if (func_contain(*it, numbers[1])) {
                    numbers[3] = *it;
                    tmp[5].erase(it);
                    break;
                }
            }

            segments[5] = func_remove(numbers[4], numbers[3]).at(0);
            segments[6] = func_remove(numbers[4], numbers[1] + segments[5]).at(0);

            // now, we can find number 5 (5 segments, including segment 5)
            for (auto& it = tmp[5].begin(); it != tmp[5].end(); it++) {
                if ((*it).find(segments[5]) != std::string::npos) {
                    numbers[5] = *it;
                    tmp[5].erase(it);
                    break;
                }
            }
            numbers[2] = tmp[5].at(0);
            segments[1] = func_remove(numbers[1], numbers[5]).at(0);
            segments[2] = func_remove(numbers[1], std::string(&segments[1], 1)).at(0);
            segments[3] = func_remove(numbers[3], numbers[7] + segments[6]).at(0);
            segments[4] = func_remove(numbers[2], numbers[3]).at(0);

            numbers[0] = std::string() + segments[0] + segments[1] + segments[2] + segments[3] + segments[4] + segments[5];
            numbers[6] = std::string() + segments[0] + segments[6] + segments[2] + segments[3] + segments[4] + segments[5];
            numbers[9] = std::string() + segments[0] + segments[1] + segments[2] + segments[3] + segments[6] + segments[5];

            for (auto& n : numbers)
                std::sort(n.begin(), n.end());

            int sum_tmp = 0;
            for (auto& o : output) {
                std::sort(o.begin(), o.end());
                for (int i = 0; i < numbers.size(); i++)
                    if (o == numbers.at(i)) {
                        sum_tmp *= 10;
                        sum_tmp += i;
                    }
            }
            sum += sum_tmp;
        }

        return sum;
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc08_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc08.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc08_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc08.txt") << std::endl;
}