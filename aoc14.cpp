#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cstdint>

#include "toolbox.h"

namespace {

    struct Input {
        std::string temp;
        std::map<std::string, std::string> mapping;

        static Input from_file(const char* filename)
        {
            Input data;
            std::ifstream in(filename);
            std::string s;
            
            std::getline(in, data.temp);
            std::getline(in, s);

            while (std::getline(in, s)) {
                auto tmp = tools::string_split(s, " -> ");
                data.mapping[tmp.at(0)] = tmp.at(1);
            }

            return data;
        }
    };

    int64_t exec(const Input &input, int cycles)
    {
        std::map<std::string, int64_t> cnt_pairs;
        std::map<char, int64_t> cnt;

        for (auto& c : input.temp)
            cnt[c]++;

        for (std::size_t pos = 0; pos < (input.temp.size() - 1); pos++)
            cnt_pairs[input.temp.substr(pos, 2)]++;

        std::map<std::string, int64_t> tmp = cnt_pairs;

        for (int i = 0; i < cycles; i++) {

            for (auto& [key, count] : cnt_pairs) {
                if (count == 0)
                    continue;
                
                auto& mapped = input.mapping.at(key);
                auto left = key.substr(0, 1) + mapped;
                auto right = mapped + key.substr(1, 1);
                
                tmp[key]            -= count;
                tmp[left]           += count;
                tmp[right]          += count;
                cnt[mapped.at(0)]   += count;
            }

            cnt_pairs = tmp;
        }

        int64_t max_c = std::numeric_limits<int64_t>::min();
        int64_t min_c = std::numeric_limits<int64_t>::max();

        for (auto& [key, value] : cnt) {
            if (value > max_c) max_c = value;
            if (value < min_c) min_c = value;
        }

        return max_c - min_c;
    }

    int64_t part1(const char* filename)
    {
        return exec(Input::from_file(filename), 10);
    }

    int64_t part2(const char* filename)
    {
        return exec(Input::from_file(filename), 40);
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc14_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc14.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc14_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc14.txt") << std::endl;
}