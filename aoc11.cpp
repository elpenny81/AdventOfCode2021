#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace {

    using Matrix = std::vector<std::vector<int>>;

    Matrix read_file(const char* filename)
    {
        Matrix matrix;
        std::ifstream in(filename);
        std::string s;
        while (in >> s) {
            std::vector<int> row;
            for (auto& c : s)
                row.push_back(static_cast<int>(c - '0'));
            matrix.push_back(row);
        }
        return matrix;
    }

    void handle(Matrix& matrix, int x, int y) {
        if (y >= matrix.size() || x >= matrix.at(0).size() || y < 0 || x < 0)
            return;
        matrix[y][x]++;
        if (matrix[y][x] == 10) {
            handle(matrix, x + 1, y);
            handle(matrix, x - 1, y);
            handle(matrix, x, y + 1);
            handle(matrix, x, y - 1);

            handle(matrix, x + 1, y + 1);
            handle(matrix, x + 1, y - 1);
            handle(matrix, x - 1, y + 1);
            handle(matrix, x - 1, y - 1);
        }
    };

    int part1(const char* filename)
    {
        auto matrix = read_file(filename);
        int sum = 0;

        for (int i = 0; i < 100; i++) {
            // step 1: add 1
            for (int y = 0; y < matrix.size(); y++)
                for (int x = 0; x < matrix.at(0).size(); x++)
                    handle(matrix, x, y);


            for (int y = 0; y < matrix.size(); y++) {
                for (int x = 0; x < matrix.at(0).size(); x++) {
                    if (matrix[y][x] >= 10) {
                        sum++;
                        matrix[y][x] = 0;
                    }
                }
            }
        }   

        return sum;
    }

    int part2(const char* filename)
    {
        auto matrix = read_file(filename);
        int step = 0;

        for (step = 0;; step++) {
            // step 1: add 1
            for (int y = 0; y < matrix.size(); y++)
                for (int x = 0; x < matrix.at(0).size(); x++)
                    handle(matrix, x, y);

            int sum = 0;
            for (int y = 0; y < matrix.size(); y++) {
                for (int x = 0; x < matrix.at(0).size(); x++) {
                    if (matrix[y][x] >= 10) {
                        sum++;
                        matrix[y][x] = 0;
                    }
                }
            }

            if (sum == 100)
                break;
        }

        return step + 1;
    }

}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc11_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc11.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc11_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc11.txt") << std::endl;
}