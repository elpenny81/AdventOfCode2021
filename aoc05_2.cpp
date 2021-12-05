#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include <algorithm>

#include "toolbox.h"

struct Point { 
    int x = 0, y = 0; 

    Point() = default;
    Point(int _x, int _y) : x(_x), y(_y) {}
    
    static Point parse(const std::string& s) {
        auto data = tools::string_split(s, ",");
        assert(data.size() == 2);
        return { std::strtol(data.at(0).c_str(), nullptr, 0), std::strtol(data.at(1).c_str(), nullptr, 0) };
    }

    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator<(const Point& other) const { return x < other.x || (x == other.x && y < other.y); };
};

struct Line { 
    Point p1, p2; 

    Line() = default;
    Line(const Point& _p1, const Point& _p2) : p1(_p1), p2(_p2) {}

    static Line parse(const std::string &line) {
        auto tokens = tools::string_split(line, " -> ");
        assert(tokens.size() == 2);
        return { Point::parse(tokens.at(0)),
                 Point::parse(tokens.at(1)) };
    }

    bool is_horizontal() const { return p1.y == p2.y; }
    bool is_vertical() const { return p1.x == p2.x; }
    bool is_diagonal() const { return !is_horizontal() && !is_vertical(); }
    std::vector<Point> to_points() const {
        std::vector<Point> points;

        auto f = [&points, this](int x, int y, int dx, int dy, auto &&f) {
            points.push_back({ x, y });
            if (x == p2.x && y == p2.y)
                return;
            f(x + dx, y + dy, dx, dy, f);
        };

        int x = p1.x, y = p1.y;
        int dx = is_horizontal() || is_diagonal() ? (x < p2.x) ? 1 : -1 : 0;
        int dy = is_vertical()   || is_diagonal() ? (y < p2.y) ? 1 : -1 : 0;

        if (dx || dy)
            f(x, y, dx, dy, f);

        return points;
    }

    inline bool operator==(const Line& other) const { return p1 == other.p1 && p2 == other.p2; }
    inline bool operator<(const Line& other) const { return p1 < other.p1 || ( p1 == other.p1 && p2 < other.p2); }
};

static std::vector<Line> read_lines(std::ifstream& in)
{
    std::vector<Line> ret;
    std::string s;
    while (std::getline(in, s))
        ret.push_back(Line::parse(s));
    return ret;
}

static std::map<Point, int> get_points_map(const std::vector<Line> &lines)
{
    std::map<Point, int> map;
    for (const auto& line : lines)
        for (auto& p : line.to_points())
            map[p]++;
    return map;
}

static int get_num_overlaps(const std::map<Point, int>& map)
{
    return static_cast<int>(std::count_if(map.begin(), map.end(), [](auto entry) { return entry.second > 1; }));
}

int main() 
{
    std::ifstream in("aoc05.txt");
    auto lines = read_lines(in);
    auto map = get_points_map(lines);
    std::cout << "Result = " << get_num_overlaps(map) << std::endl;
}