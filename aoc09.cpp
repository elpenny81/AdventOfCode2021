#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

namespace {
    struct Point {
        int x = 0, y = 0;

        Point() = default;
        Point(int _x, int _y) : x(_x), y(_y) {}
        bool operator==(const Point& other) const { return x == other.x && y == other.y; }
        bool operator<(const Point& other) const { return x < other.x || (x == other.x && y < other.y); };
    };

    std::vector<std::vector<char>> get_map(std::ifstream &in)
    {
        std::vector<std::vector<char>> ret;
        std::string line;
        while (std::getline(in, line)) {
            std::vector<char> chars;
            for (auto &c : line)
                chars.push_back(c);
            ret.push_back(chars);
        }
        return ret;
    }

    int part1(const char *filename)
    {
        std::ifstream in(filename);
        auto data = get_map(in);
        int y_size = data.size();
        int x_size = data.at(0).size();
        int sum = 0;

        for (size_t x = 0; x < x_size; x++) {
            for (size_t y = 0; y < y_size; y++) {
                int left = x - 1, right = x + 1, up = y - 1, down = y + 1;
                if (   (left < 0        || data[y][x] < data[y][left])
                    && (right >= x_size || data[y][x] < data[y][right])
                    && (up < 0          || data[y][x] < data[up][x])
                    && (down >= y_size  || data[y][x] < data[down][x])) {
                    sum += data[y][x] - '0' + 1;
                }
            }
        }

        return sum;
    }

    using BasinMatrix=std::map<Point, int>;

    void calculate_basin(const std::vector<std::vector<char>> &map, int x, int y, BasinMatrix &matrix)
    {
        const int y_size = map.size();
        const int x_size = map.at(0).size();
        matrix[{x, y}] = 1;

        char v     = map[y][x];
        char v_l   = (x > 0)                        ? map[y][x-1]   : '9';
        char v_r   = (x < x_size-1)                 ? map[y][x+1]   : '9';
        char v_u   = (y > 0)                        ? map[y-1][x]   : '9';
        char v_d   = (y < y_size-1)                 ? map[y+1][x]   : '9';

        if (v_l != '9' && v_l > v) calculate_basin(map, x - 1, y, matrix);
        if (v_r != '9' && v_r > v) calculate_basin(map, x + 1, y, matrix);
        if (v_u != '9' && v_u > v) calculate_basin(map, x, y - 1, matrix);
        if (v_d != '9' && v_d > v) calculate_basin(map, x, y + 1, matrix);
    }

    int part2(const char *filename)
    {
        std::ifstream in(filename);
        auto data = get_map(in);
        int y_size = data.size();
        int x_size = data.at(0).size();
        std::vector<int> basins;
        BasinMatrix matrix;
        int sum = 1;

        for (size_t x = 0; x < x_size; x++) {
            for (size_t y = 0; y < y_size; y++) {
                int left = x - 1, right = x + 1, up = y - 1, down = y + 1;
                if (   (left < 0        || data[y][x] < data[y][left])
                    && (right >= x_size || data[y][x] < data[y][right])
                    && (up < 0          || data[y][x] < data[up][x])
                    && (down >= y_size  || data[y][x] < data[down][x])) {
                    BasinMatrix matrix;
                    calculate_basin(data, x, y, matrix);
                    basins.push_back(matrix.size());
                }
            }
        }

        std::sort(basins.begin(), basins.end());
        for (auto it = basins.end() - 3; it != basins.end(); it++)
            sum *= *it;

        return sum;
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc09_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc09.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc09_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc09.txt") << std::endl;
}
