#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static int get_weight(const std::vector<std::string> &input, std::size_t pos)
{
    int weight = 0;
    for (const auto &d : input) {
        if      (d.at(pos) == '0') weight--;
        else if (d.at(pos) == '1') weight++;
    }
    return weight;
}

static std::vector<std::string> filter_vec(const std::vector<std::string> &input, std::size_t pos, bool bit_value)
{
    std::vector<std::string> out;
    char c = bit_value ? '1' : '0';
    for (auto &e : input) {
        if (e.at(pos) == c)
            out.push_back(e);
    }
    return out;
}

static int get_oxygen_gen_rating(const std::vector<std::string> &input)
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

static int get_co2_scrubber_rating(const std::vector<std::string> &input)
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

int main()
{
    std::ifstream input("aoc03.txt");
    std::string line;

    std::vector<std::string> data;
    while (input >> line)
        data.push_back(line);

    int oxygen = get_oxygen_gen_rating(data);
    int co2 = get_co2_scrubber_rating(data);

    std::cout << "Result = " << (oxygen * co2) << std::endl;
}
