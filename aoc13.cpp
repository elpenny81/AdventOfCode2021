#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#include "toolbox.h"

namespace {

    struct Coord {
        int x = 0, y = 0;

        Coord() = default;
        Coord(int _x, int _y) : x(_x), y(_y) {}

        static Coord from_string(const std::string& s) {
            Coord c;
            auto data = tools::string_split(s, ",");
            c.x = atoi(data.at(0).c_str());
            c.y = atoi(data.at(1).c_str());
            return c;
        }

        bool operator<(const Coord& o) const {
            return x < o.x || (x == o.x && y < o.y);
        }

        bool operator==(const Coord& o) const {
            return x == o.x && y == o.y;
        }
    };

    enum class FoldDirection {
        x, y
    };

    struct Fold {
        FoldDirection direction = FoldDirection::y;
        int steps = 0;

        static Fold from_string(const std::string& s) {
            Fold fold;
            auto data = tools::string_split(s, "=");
            if (data.at(0) == "x")
                fold.direction = FoldDirection::x;
            else if (data.at(0) == "y")
                fold.direction = FoldDirection::y;

            fold.steps = atoi(data.at(1).c_str());
            return fold;
        }
    };

    struct InputData {
        std::set<Coord> coordinates;
        std::vector<Fold>  folds;

        static InputData from_file(const char* filename) {
            InputData data;

            std::ifstream in(filename);
            std::string s;
            while (std::getline(in, s)) {
                if (s.empty())
                    continue;
                std::size_t pos = s.find("fold along ");
                if (pos == std::string::npos) {
                    data.coordinates.insert(Coord::from_string(s));
                } else {
                    data.folds.push_back(Fold::from_string(s.substr(pos + strlen("fold along "))));
                }
            }

            return data;
        }
    };

    int part1(const char* filename)
    {
        auto data = InputData::from_file(filename);

        // Execute first fold
        std::set<Coord> result;
        auto &fold = data.folds.at(0);

        for (auto& c : data.coordinates) {
            if (fold.direction == FoldDirection::x) {
                if (c.x > fold.steps) {
                    result.insert({ c.x - (c.x - fold.steps) * 2, c.y });
                }
                else if (c.x < fold.steps) {
                    result.insert(c);
                }
            }
            else if (fold.direction == FoldDirection::y) {
                if (c.y > fold.steps) {
                    result.insert({ c.x, c.y - (c.y - fold.steps) * 2 });
                }
                else if (c.y < fold.steps) {
                    result.insert(c);
                }
            }
        }

        return int(result.size());
    }

    void dump_coordinates(const std::set<Coord>& coordinates)
    {
        int min_x = 99999999, min_y = 99999999;
        int max_x = -99999999, max_y = -99999999;

        for (auto& c : coordinates) {
            if (c.x < min_x) min_x = c.x;
            if (c.y < min_y) min_y = c.y;
            if (c.x > max_x) max_x = c.x;
            if (c.y > max_y) max_y = c.y;
        }

        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                if (std::find(coordinates.begin(), coordinates.end(), Coord{ x,y }) != coordinates.end())
                    std::cout << "#";
                else
                    std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    int part2(const char* filename)
    {
        auto data = InputData::from_file(filename);

        // Execute first fold
        std::set<Coord> tmp = data.coordinates;
        std::set<Coord> result;

        for (auto& fold : data.folds) {

            for (auto& c : tmp) {
                if (fold.direction == FoldDirection::x) {
                    if (c.x > fold.steps) {
                        result.insert({ c.x - (c.x - fold.steps) * 2, c.y });
                    }
                    else if (c.x < fold.steps) {
                        result.insert(c);
                    }
                }
                else if (fold.direction == FoldDirection::y) {
                    if (c.y > fold.steps) {
                        result.insert({ c.x, c.y - (c.y - fold.steps) * 2 });
                    }
                    else if (c.y < fold.steps) {
                        result.insert(c);
                    }
                }
            }

            tmp = result;
            result.clear();
        }

        std::cout << std::endl;
        dump_coordinates(tmp);

        return 0;
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc13_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc13.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc13_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc13.txt") << std::endl;
}