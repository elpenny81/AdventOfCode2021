#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <cstdint>

#include "toolbox.h"

static uint64_t get_fishes_after(int days, const std::vector<uint64_t> &input)
{
    std::vector<uint64_t> days_fish_map(9); // size of 9, for each day one slot
    for (auto &e : input)
        days_fish_map[e]++;

    for (int day = 0; day < days; day++) {
        std::vector<uint64_t> tmp(days_fish_map.begin() + 1, days_fish_map.end());
        tmp[6] += days_fish_map[0];
        tmp.push_back(days_fish_map[0]);
        days_fish_map = tmp;
    }

    uint64_t sum = 0;
    return std::accumulate(days_fish_map.begin(), days_fish_map.end(), sum);
}

static std::vector<uint64_t> read_input_vector(const std::string &filename)
{
    std::ifstream in(filename);
    std::string line;
    std::getline(in, line);
    auto string_list = tools::string_split(line, ",");
    std::vector<uint64_t> input;
    for (auto &e : string_list)
        input.push_back(std::strtoull(e.c_str(), nullptr, 0));
    return input;
}

int main()
{
    auto input_sample = read_input_vector("aoc06_sample.txt");
    auto input = read_input_vector("aoc06.txt");

    std::cout << "Result (part1, demo) = " << get_fishes_after(80, input_sample) << std::endl;
    std::cout << "Result (part1, real) = " << get_fishes_after(80, input) << std::endl;
    std::cout << "Result (part2, demo) = " << get_fishes_after(256, input_sample) << std::endl;
    std::cout << "Result (part2, real) = " << get_fishes_after(256, input) << std::endl;
}
