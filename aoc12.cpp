#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "toolbox.h"

namespace {

    struct Node {
        bool start = false;
        bool end = false;
        bool big = false;
        bool small = false;

        std::string name;
        std::map<std::string, Node *> connected_nodes;

        static std::shared_ptr<Node> create(const std::string& s) {
            std::shared_ptr<Node> node (new Node);
            node->name = s;
            if (s == "start") node->start = true;
            else if (s == "end")   node->end = true;
            else if (s.size() > 0 && s.at(0) >= 'A' && s.at(0) <= 'Z') node->big = true;
            else node->small = true;
            return node;
        }
    };

    std::map<std::string, std::shared_ptr<Node>> get_mesh(const char* filename)
    {
        std::ifstream in(filename);
        std::string s;
        std::map<std::string, std::shared_ptr<Node>> nodes;

        while (in >> s) {
            auto conn = tools::string_split(s, "-");
            if (nodes.find(conn.at(0)) == nodes.end())
                nodes[conn.at(0)] = Node::create(conn.at(0));
            if (nodes.find(conn.at(1)) == nodes.end())
                nodes[conn.at(1)] = Node::create(conn.at(1));
            nodes[conn.at(0)]->connected_nodes[conn.at(1)] = nodes[conn.at(1)].get();
            nodes[conn.at(1)]->connected_nodes[conn.at(0)] = nodes[conn.at(0)].get();
        }

        return nodes;
    }

    void find_path(Node* node, std::map<std::string, int> cnt, int &num_paths, int small_allowed)
    {
        cnt[node->name]++;
        if (node->small && small_allowed == 2 && cnt[node->name] == 2)
            small_allowed = 1;

        if (node->end) {
            num_paths++;
            return;
        }

        for (auto& [key, n] : node->connected_nodes) {
            if (n->end || n->big || (n->small && cnt[n->name] < small_allowed))
                find_path(n, cnt, num_paths, small_allowed);
        };
    }

    int part1(const char* filename)
    {
        int num_paths = 0;
        auto nodes = get_mesh(filename);
        find_path(nodes["start"].get(), {}, num_paths, 1);
        return num_paths;
    }

    int part2(const char* filename)
    {
        int num_paths = 0;
        auto nodes = get_mesh(filename);
        find_path(nodes["start"].get(), {}, num_paths, 2);
        return num_paths;
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc12_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc12.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc12_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc12.txt") << std::endl;
}