#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <memory>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>

namespace {

    struct Element {

        enum class Type { Literal, Pair } type = Type::Literal;
        enum class Direction { Left, Right };
        
        Element* parent = nullptr;

        std::shared_ptr<Element> left;
        std::shared_ptr<Element> right;
        
        int value = -1;

        Element() = default;
        
        static std::shared_ptr<Element> make_literal(int value, Element *parent = nullptr)
        {
            auto obj = std::make_shared<Element>();
            obj->type = Type::Literal;
            obj->value = value;
            obj->parent = parent;
            return obj;
        }
        
        static std::shared_ptr<Element> make_pair(
            const std::shared_ptr<Element>& left,
            const std::shared_ptr<Element>& right)
        {
            auto obj = std::make_shared<Element>();
            obj->type = Type::Pair;
            obj->left = left;
            obj->right = right;
            obj->left->parent = obj.get();
            obj->right->parent = obj.get();
            return obj;
        }

        int level() const {
            int i = 0;
            Element* p;
            for (i = 0, p = parent; p; i++, p = p->parent);
            return i;
        }

        bool add_literal(int v, Direction direction) {
            if (type == Type::Literal) {
                value += v;
                return true;
            }
            else if (direction == Direction::Left && left) {
                return left->add_literal(v, direction);
            }
            else if (direction == Direction::Right && right) {
                return right->add_literal(v, direction);
            }

            return false;
        }

        bool signal_explode_left(Element* sender, int v) {
            if (left.get() == sender) {
                if (parent)
                    parent->signal_explode_left(this, v);
                return false;
            }
            else
            if (left->type == Type::Literal) {
                left->value += v;
                return true;
            }
            else
            if (left->type == Type::Pair) {
                return left->add_literal(v, Direction::Right);
            }

            return false;
        }

        bool signal_explode_right(Element* sender, int v) {
            if (right.get() == sender) {
                if (parent)
                    parent->signal_explode_right(this, v);
                return false;
            }
            else
            if (right->type == Type::Literal) {
                right->value += v;
                return true;
            }
            else
            if (right->type == Type::Pair) {
                return right->add_literal(v, Direction::Left);
            }

            return false;
        }

        bool signal_explode(Element *sender, Direction direction, int v) {
            if (direction == Direction::Left)
                return signal_explode_left(sender, v);
            return signal_explode_right(sender, v);
        }

        void explode() {
            if (type == Type::Pair && parent) {
                // keep alive
                auto data = (parent->left.get() == this) ? parent->left : parent->right;

                if (!parent->signal_explode(this, Direction::Left, left->value))
                    parent->left = make_literal(0, parent);
                if (!parent->signal_explode(this, Direction::Right, right->value))
                    parent->right = make_literal(0, parent);
            }
        }

        void split() {
            if (type == Type::Literal) {
                left = make_literal(static_cast<int>(floor(value / 2.0)), this);
                right = make_literal(static_cast<int>(ceil(value / 2.0)), this);
                type = Type::Pair;
            }
        }

        bool check_explodes() {
            if (type == Type::Pair) {
                if (level() >= 4 && left->type == Type::Literal && right->type == Type::Literal) {
                    explode();
                    return false;
                }

                return left->check_explodes() && right->check_explodes();
            }

            return true;
        }

        bool check_splits() {
            if (type == Type::Literal) {
                if (value >= 10) {
                    split();
                    return false;
                }

                return true;
            }

            return left->check_splits() && right->check_splits();
        }

        int64_t get_magnitude() const {
            int64_t left_value = left
                               ? left->type == Type::Literal
                               ? left->value
                               : left->get_magnitude()
                               : 0;
            int64_t right_value = right
                                ? right->type == Type::Literal
                                ? right->value
                                : right->get_magnitude()
                                : 0;

            return left_value * 3 + right_value * 2;
        }

        static std::shared_ptr<Element> addition(
            const std::shared_ptr<Element>& left,
            const std::shared_ptr<Element>& right
        ) {
            return make_pair(left, right);
        }

        static std::shared_ptr<Element> from_string(const std::string& s)
        {
            if (s.at(0) == '[') {

                int bracket_cnt = 0;

                // find delimiter
                std::size_t pos;
                for (pos = 1; pos < s.length(); pos++) {
                    if (s.at(pos) == '[')
                        bracket_cnt++;
                    if (s.at(pos) == ']')
                        bracket_cnt--;
                    if (s.at(pos) == ',' && bracket_cnt == 0)
                        break;
                }

                std::size_t end_of_pair = s.find_last_of(']');

                return make_pair(
                    from_string(s.substr(1, pos - 1)), 
                    from_string(s.substr(pos + 1, end_of_pair - (pos + 1)))
                );
            }

            // we have a literal
            return make_literal(atoi(s.c_str()));
        }

        std::string to_string() const {
            std::stringstream s;
            if (type == Type::Pair)
                s << "[" << left->to_string() << "," << right->to_string() << "]";
            else
                s << value;
            return s.str();
        }

        std::shared_ptr<Element> copy() const {
            // expensive copy, but I'm too lazy to implement a proper solution ;o)
            return from_string(to_string());
        }
    };


    int64_t part1(const char* filename)
    {
        std::ifstream in(filename);
        std::string s;
        std::shared_ptr<Element> root;

        while (in >> s) {
            if (root)
                root = Element::addition(root, Element::from_string(s));
            else
                root = Element::from_string(s);

            while (!root->check_explodes() || !root->check_splits()) {
                // std::cout << root->to_string() << std::endl;
            }
        }

        return root->get_magnitude();
    }

    int64_t part2(const char* filename)
    {
        std::ifstream in(filename);
        std::string s;
        std::vector<std::shared_ptr<Element>> elements;
        std::vector<int64_t> magnitudes;

        while (in >> s)
            elements.push_back(Element::from_string(s));

        for (auto& e1 : elements) {
            for (auto& e2 : elements) {
                if (e1 == e2) continue;
                auto e = Element::addition(e1->copy(), e2->copy());

                while (!e->check_explodes() || !e->check_splits());
                magnitudes.push_back(e->get_magnitude());
            }
        }

        return *std::max_element(magnitudes.begin(), magnitudes.end());
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc18_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc18.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc18_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc18.txt") << std::endl;
}