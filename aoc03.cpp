#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace {

    int get_weight(const std::vector<std::string>& input, std::size_t pos)
    {
        int weight = 0;
        for (const auto& d : input) {
            if (d.at(pos) == '0') weight--;
            else if (d.at(pos) == '1') weight++;
        }
        return weight;
    }

    std::vector<std::string> filter_vec(const std::vector<std::string>& input, std::size_t pos, bool bit_value)
    {
        std::vector<std::string> out;
        char c = bit_value ? '1' : '0';
        for (auto& e : input) {
            if (e.at(pos) == c)
                out.push_back(e);
        }
        return out;
    }

    int get_oxygen_gen_rating(const std::vector<std::string>& input)
    {
        auto length = input.at(0).length();
        std::vector<std::string> work = input;

        while (work.size() > 1) {
            for (size_t i = 0; i < length && work.size() > 1; i++) {
                int weight = get_weight(work, i);
                if (weight >= 0) work = filter_vec(work, i, true);
                else             work = filter_vec(work, i, false);
            }
        }

        int ret = 0;
        for (std::size_t i = 0; i < length; i++)
            ret = ret << 1 | work.at(0).at(i) - '0';
        return ret;
    }

    int get_co2_scrubber_rating(const std::vector<std::string>& input)
    {
        auto length = input.at(0).length();
        std::vector<std::string> work = input;

        while (work.size() > 1) {
            for (size_t i = 0; i < length && work.size() > 1; i++) {
                int weight = get_weight(work, i);
                if (weight >= 0) work = filter_vec(work, i, false);
                else             work = filter_vec(work, i, true);
            }
        }

        int ret = 0;
        for (std::size_t i = 0; i < length; i++)
            ret = ret << 1 | work.at(0).at(i) - '0';
        return ret;
    }

    int part1(const char* filename)
    {
        std::ifstream input(filename);
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

        return gamma * epsilon;
    }

    int part2(const char* filename)
    {
        std::ifstream input(filename);
        std::string line;

        std::vector<std::string> data;
        while (input >> line)
            data.push_back(line);

        int oxygen = get_oxygen_gen_rating(data);
        int co2 = get_co2_scrubber_rating(data);

        return oxygen * co2;
    }
};

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc03_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc03.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc03_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc03.txt") << std::endl;
}
