#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <map>

#include "toolbox.h"

struct Point { 
    int x = 0, y = 0; 

    Point() = default;
    Point(int _x, int _y) : x(_x), y(_y) {}
    
    static Point parse(const std::string& s) {
        Point point;
        auto data = tools::string_split(s, ",");
        assert(data.size() == 2);
        point.x = std::strtol(data.at(0).c_str(), nullptr, 0);
        point.y = std::strtol(data.at(1).c_str(), nullptr, 0);
        return point;
    }

    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator<(const Point& other) const { return x < other.x || (x == other.x && y < other.y); };
};

struct Line { 
    Point p1, p2; 

    static Line parse(const std::string &line) {
        Line ret;
        auto tokens = tools::string_split(line, " ");
        assert(tokens.size() == 3);
        ret.p1 = Point::parse(tokens.at(0));
        ret.p2 = Point::parse(tokens.at(2));
        return ret;
    }

    bool is_horizontal() const { return p1.y == p2.y; }
    bool is_vertical() const { return p1.x == p2.x; }
    std::vector<Point> to_points() const {
        std::vector<Point> points;
        if (is_horizontal()) {
            int i = p1.x;
            while (true) {
                points.push_back(Point(i, p1.y));
                if (i == p2.x)
                    break;
                i += (i > p2.x) ? (-1) : (1);
            }
        }
        else
        if (is_vertical()) {
            int i = p1.y;
            while (true) {
                points.push_back(Point(p1.x, i));
                if (i == p2.y)
                    break;
                i += (i > p2.y) ? (-1) : (1);
            }
        }

        return points;
    }

    inline bool operator==(const Line& other) const { return p1 == other.p1 && p2 == other.p2; }
    inline bool operator<(const Line& other) const { return p1 < other.p1 || ( p1 == other.p1 && p2 < other.p2); }
};

static std::vector<Line> read_lines(std::ifstream& in)
{
    std::vector<Line> ret;
    std::string s;
    while (std::getline(in, s)) {
        auto line = Line::parse(s);
        ret.push_back(line);
    }
    return ret;
}

static std::map<Point, int> get_points_map(const std::vector<Line> &lines)
{
    std::map<Point, int> map;
    for (const auto& line : lines) {
        auto points = line.to_points();
        for (auto& p : points)
            map[p]++;
    }
    return map;
}

static int get_num_overlaps(const std::map<Point, int>& map)
{
    int n = 0;
    for (auto& [point, count] : map) {
        if (count > 1)
            n++;
    }
    return n;
}

int main() 
{
    std::ifstream in("aoc05.txt");
    auto lines = read_lines(in);
    auto map = get_points_map(lines);
    std::cout << "Result = " << get_num_overlaps(map) << std::endl;
}