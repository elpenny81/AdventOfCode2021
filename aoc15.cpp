#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>

namespace {

    struct Coordinate {
        int x = 0, y = 0;

        Coordinate() = default;
        Coordinate(int x, int y) : x(x), y(y) {}

        bool operator==(const Coordinate& other) const { return x == other.x && y == other.y; }
        bool operator<(const Coordinate& other) const { return x < other.x || (x == other.x && y < other.y); }
    };

    struct Status {
        int f = 0, g = 0, h = 0;
    };

    struct Node {

        Coordinate coord;
        Status status;
        std::vector<Node *> neighbors;
        bool initialized = false;
        bool closed = false;

        Node() = default;
        Node(const Coordinate& c) : coord(c) { }

        Node* parent = nullptr;
    };

    struct Map {
        std::map<Coordinate, std::shared_ptr<Node>> nodes;
        std::map<Coordinate, int> g_values;
        int x_size = 0;
        int y_size = 0;

        static Map parse(const char* filename) {
            Map map;
            std::ifstream in(filename);
            std::string s;
            Coordinate c;
            while (in >> s) {
                for (auto& ch : s) {
                    map.nodes[c] = std::make_shared<Node>(c);
                    map.g_values[c] = static_cast<int>(ch - '0');
                    c.x++;
                }
                map.x_size = c.x; c.y++; c.x = 0;
            }
            map.y_size = c.y;
            return map;
        }
        
        Node* get(const Coordinate& c) const {
            return nodes.at(c).get();
        }

        std::vector<Node *> neighbors(Node *ptr) const {

            auto c = ptr->coord;
            if (!ptr->initialized) {
                if ((c.x + 1) < x_size) ptr->neighbors.push_back(nodes.at({ c.x + 1, c.y }).get());
                if ((c.x - 1) >= 0)     ptr->neighbors.push_back(nodes.at({ c.x - 1, c.y }).get());
                if ((c.y + 1) < y_size) ptr->neighbors.push_back(nodes.at({ c.x, c.y + 1 }).get());
                if ((c.y - 1) >= 0)     ptr->neighbors.push_back(nodes.at({ c.x, c.y - 1 }).get());
                ptr->initialized = true;
            }
            return ptr->neighbors;
        }
    };


    struct AStar {
        std::list<Node *> openlist;
        Map map;

        AStar(const Map& map) : map(map) {}

        Node *nodeWithLowestF() const {
            auto ret = openlist.front();
            for (auto& n : openlist)
                if (n->status.f < ret->status.f)
                    ret = n;
            return ret;
        }

        void operator()(const Coordinate &target) {

            auto start = map.get({ 0, 0 });
            openlist.push_back(map.get({0,0}));
            auto target_node = map.get(target);

            while (openlist.size()) {
                auto current = nodeWithLowestF();
                openlist.erase(std::remove(openlist.begin(), openlist.end(), current), openlist.end());
                current->closed = true;

                for (auto& child : map.neighbors(current)) {

                    if (child->closed)
                        continue;

                    int h = 1;
                    int g = current->status.g + map.g_values.at(child->coord);
                    int f = h + g;

                    auto pos = std::find(openlist.begin(), openlist.end(), child);
                    if (pos == openlist.end()) {
                        child->status.f = f;
                        child->status.g = g;
                        child->status.h = h;
                        child->parent = current;
                        openlist.push_back(child);
                    } else {
                        if (g < (*pos)->status.g) {
                            child->status.f = f;
                            child->status.g = g;
                            child->status.h = h;
                            child->parent = current;
                        }
                    }
                }

                if (target_node->closed)
                    return;
            };
        }
    };


    int part1(const char* filename)
    {
        auto map = Map::parse(filename);
        Coordinate target{ map.x_size - 1, map.y_size - 1 };
        AStar a(map);
        a(target);
        return map.get(target)->status.g;
    }

    int part2(const char* filename)
    {
        auto map = Map::parse(filename);
        // enhance map
        for (int i = 0; i < map.x_size * 5; i++) {
            for (int j = 0; j < map.y_size * 5; j++) {

                Coordinate c(i, j);
                if (map.nodes.find(c) != map.nodes.end())
                    continue;

                map.nodes[c] = std::make_shared<Node>(c);

                int x, y;

                if (c.x >= map.x_size) {
                    x = c.x - map.x_size;
                    y = c.y;
                }
                else {
                    x = c.x;
                    y = c.y - map.y_size;
                }
                
                int value = map.g_values[{ x, y }] + 1;
                if (value > 9) value = 1;
                map.g_values[c] = value;
            }
        }
        map.x_size *= 5;
        map.y_size *= 5;
        
        Coordinate target{ map.x_size - 1, map.y_size - 1 };
        AStar a(map);
        a(target);
        return map.get(target)->status.g;
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc15_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc15.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc15_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc15.txt") << std::endl;
}