#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

namespace {

    struct Number {
        int number = 0;
        bool marked = false;
    };

    struct Board {
        bool complete = false;
        Number numbers[5][5];
        static Board parse(std::ifstream& stream) {
            Board board;
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                    stream >> board.numbers[i][j].number;
            return board;
        }

        void mark_number(int n) {
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                    if (numbers[i][j].number == n)
                        numbers[i][j].marked = true;
        }

        bool check_rows() const
        {
            for (int i = 0; i < 5; i++) {
                bool test = true;
                for (int j = 0; j < 5 && test; j++)
                    test &= numbers[i][j].marked;

                if (test)
                    return true;
            }

            return false;
        }

        bool check_columns() const
        {
            for (int i = 0; i < 5; i++) {
                bool test = true;
                for (int j = 0; j < 5 && test; j++)
                    test &= numbers[j][i].marked;

                if (test)
                    return true;
            }

            return false;
        }

        bool check_bingo() const {
            return check_rows() || check_columns();
        }
    };

    std::vector<int> parse_drawn_numbers(const std::string& line)
    {
        std::vector<int> ret;
        const std::string delimiter = ",";
        std::string token;
        std::string s = line;
        size_t pos = 0;

        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            ret.push_back(std::strtol(token.c_str(), nullptr, 0));
            s.erase(0, pos + delimiter.length());
        }

        return ret;
    }

    auto parse_file(const std::string& filename)
    {
        std::ifstream in(filename);
        std::string line;

        std::getline(in, line);
        std::vector<int> drawn_numbers = parse_drawn_numbers(line);

        std::vector<Board> boards;
        while (std::getline(in, line)) {
            // line is empty next 5 lines are the board definition
            boards.push_back(Board::parse(in));
        }

        return std::make_tuple(drawn_numbers, boards);
    }

    int unmarked(const Board& winner_board)
    {
        int unmarked_sum = 0;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (!winner_board.numbers[i][j].marked)
                    unmarked_sum += winner_board.numbers[i][j].number;
            }
        }
        return unmarked_sum;
    }

    int part1(const char* filename)
    {
        auto [drawn_numbers, boards] = parse_file(filename);

        for (auto& drawn : drawn_numbers) {
            for (auto& board : boards) {
                board.mark_number(drawn);
                if (board.check_bingo())
                    return unmarked(board) * drawn;
            }
        }

        return 0;
    }

    int part2(const char* filename)
    {
        auto [drawn_numbers, boards] = parse_file(filename);

        int last_drawn = 0;
        Board last_win_board;

        for (auto& drawn : drawn_numbers) {
            for (auto& board : boards) {
                if (board.complete)
                    continue;

                board.mark_number(drawn);
                if (board.check_bingo()) {
                    board.complete = true;
                    last_drawn = drawn;
                    last_win_board = board;
                }
            }
        }

        return unmarked(last_win_board) * last_drawn;
    }
};

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc04_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc04.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc04_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc04.txt") << std::endl;
}