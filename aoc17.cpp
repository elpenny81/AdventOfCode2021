#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>

#include "toolbox.h"

namespace {

    struct Point {
        int x = 0, y = 0;

        Point() = default;
        Point(int x, int y) : x(x), y(y) {}

        bool operator==(const Point& o) const { return x == o.x && y == o.y; }
        bool operator<(const Point& o) const { return x < o.x || (x == o.x && y < o.y); }

        Point operator+(const Point& o) { 
            Point p(x + o.x, y + o.y);
            return p; 
        }

        Point& operator+=(const Point& o) {
            x += o.x; y += o.y;
            return *this;
        }

        Point& operator=(const Point& o) {
            x = o.x; y = o.y;
            return *this;
        }
    };

    struct Area {
        Point p1, p2;

        bool inArea(const Point& p) const {
            return p.x >= p1.x
                && p.x <= p2.x
                && p.y <= p1.y
                && p.y >= p2.y;
        }

        static Area parse(const char* filename) {
            std::ifstream in(filename);
            std::string s;
            std::getline(in, s);
            auto pos_x = s.find("x", 0);
            s = s.substr(pos_x);

            auto ranges = tools::string_split(s, ", ");
            auto x_ranges_str = tools::string_split(ranges.at(0), "=").at(1);
            auto y_ranges_str = tools::string_split(ranges.at(1), "=").at(1);
            auto x_range = tools::string_split(x_ranges_str, "..");
            auto y_range = tools::string_split(y_ranges_str, "..");

            Area a;
            a.p1 = Point(atoi(x_range.at(0).c_str()), atoi(y_range.at(1).c_str()));
            a.p2 = Point(atoi(x_range.at(1).c_str()), atoi(y_range.at(0).c_str()));
            return a;
        }
    };

    struct SimLoop {
        
        Point m_current;
        Point m_velocity;

        int m_y_max = -9999;

        SimLoop(const Point& startVelocity) :
            m_velocity(startVelocity)
        {

        }

        void update()
        {
            m_current += m_velocity;

            if (m_current.y > m_y_max)
                m_y_max = m_current.y;

            if (m_velocity.x > 0)
                m_velocity.x -= 1;
            m_velocity.y -= 1;
        }
    };

    int64_t part1(const char* filename)
    {
        auto area = Area::parse(filename);
        int result = -99999;
        
        for (int x = 0; x < area.p2.x + 1; x++) {
            for (int y = area.p2.y; y < abs(area.p2.y); y++) {
                Point startVel(x, y);
                auto sim = SimLoop(startVel);

                while (true) {
                    sim.update();

                    if (area.inArea(sim.m_current)) {
                        if (sim.m_y_max > result)
                            result = sim.m_y_max;
                        break;
                    }

                    if (sim.m_current.x > area.p2.x || sim.m_current.y < area.p2.y)
                        break;
                }
            }
        }

        return result;
    }

    int64_t part2(const char* filename)
    {
        auto area = Area::parse(filename);

        int result = 0;

        for (int x = 0; x < area.p2.x + 1; x++) {
            for (int y = area.p2.y; y < abs(area.p2.y); y++) {
                Point startVel(x, y);
                auto sim = SimLoop(startVel);

                while (true) {
                    sim.update();

                    if (area.inArea(sim.m_current)) {
                        result++;
                        break;
                    }

                    if (sim.m_current.x > area.p2.x || sim.m_current.y < area.p2.y)
                        break;
                }
            }
        }

        return result;
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc17_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc17.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc17_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc17.txt") << std::endl;
}