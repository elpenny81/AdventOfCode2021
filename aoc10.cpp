#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <cstdint>

#include "toolbox.h"

namespace {
    int64_t part1(const char* filename)
    {
        int64_t score = 0;
        const std::vector<char> opening = { '(', '[', '{', '<' };
        const std::vector<char> closing = { ')', ']', '}', '>' };
        const std::map<char, int> score_table = {
            {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}
        };
        
        std::ifstream in(filename);
        std::string line;

        while (in >> line) {
            for (auto& c : line) {

                std::stack<char> stack;

                if (std::find(opening.begin(), opening.end(), c) != opening.end())
                    stack.push(c);
                std::vector<char>::const_iterator it;
                if ((it = std::find(closing.begin(), closing.end(), c)) != closing.end()) {
                    int64_t index = std::distance(closing.begin(), it);
                    if (stack.empty() || stack.top() != opening[index]) {
                        score += score_table.at(c);
                        break;
                    }
                    stack.pop();
                }
            }
        }

        return score;
    }

    int64_t part2(const char* filename)
    {
        const std::vector<char> opening = { '(', '[', '{', '<' };
        const std::vector<char> closing = { ')', ']', '}', '>' };
        const std::map<char, int> score_table = {
            {')', 1}, {']', 2}, {'}', 3}, {'>', 4}
        };

        std::vector<int64_t> scores;
        std::ifstream in(filename);
        std::string line;

        while (in >> line) {

            int64_t score = 0;
            std::stack<char> stack;
            bool corrupt = false;

            for (auto& c : line) {
                if (std::find(opening.begin(), opening.end(), c) != opening.end())
                    stack.push(c);
                std::vector<char>::const_iterator it;
                if ((it = std::find(closing.begin(), closing.end(), c)) != closing.end()) {
                    int64_t index = std::distance(closing.begin(), it);
                    // skip corrupted lines
                    if (stack.empty() || stack.top() != opening[index]) {
                        corrupt = true;
                        break;
                    }
                    stack.pop();
                }
            }

            if (corrupt)
                continue;

            // auto-complete
            while (!stack.empty()) {
                auto c = stack.top();
                auto close_c = closing.at(tools::index_of(opening, c));
                stack.pop();
                score = score * 5 + score_table.at(close_c);
            }

            scores.push_back(score);
        }

        // middle score
        std::sort(scores.begin(), scores.end());
        return scores.at(scores.size() / 2);
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc10_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc10.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc10_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc10.txt") << std::endl;
}