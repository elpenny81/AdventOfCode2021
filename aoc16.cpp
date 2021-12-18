#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>

namespace {

    struct BitReader {

        std::vector<uint8_t> byteArray;
        int byteOffset = 0;
        int m_capacity = 0;
        uint8_t m_buffer = 0;
        int m_bitcnt = 0;

        BitReader(const std::vector<uint8_t>& byteArray) : byteArray(byteArray) {}

        bool readBuffer()
        {
            if (byteOffset >= byteArray.size())
                return false;

            m_capacity = 8;
            m_buffer = byteArray.at(byteOffset++);
            return true;
        }

        void align() {
            if (m_capacity < 8) {
                m_bitcnt += 8 - m_capacity;
                readBuffer();
            }
        }

        bool canRead() const { return byteOffset < byteArray.size(); }

        bool read_bits(int num_bits, uint64_t*b) {

            bool status = true;
            m_bitcnt += num_bits;

            if (num_bits <= m_capacity) {
                m_capacity = uint8_t(m_capacity - num_bits);
                *b = uint64_t((m_buffer >> m_capacity) & (0xff >> (8 - num_bits)));
            }
            else {
                *b = uint64_t(m_buffer & (0xff >> (8 - m_capacity)));
                num_bits = (num_bits - m_capacity);
                m_capacity = 0;

                while (status && num_bits >= 8) {
                    status = readBuffer();
                    *b = ((*b) << 8) | m_buffer;
                    num_bits = num_bits - 8;
                    m_capacity = 0;
                }

                if (status && num_bits > 0) {
                    if ((status = readBuffer())) {
                        *b = ((*b) << num_bits | uint8_t(m_buffer >> (8 - num_bits)));
                        m_capacity = uint8_t(8 - num_bits);
                    }
                }
            }

            return status;
        }
    };

    struct Result {
        uint64_t part1 = 0;
        uint64_t operationResult = 0;
    };

    enum class Operation {
        Sum,
        Product,
        Minimum,
        Maximum,
        Literal,
        GreaterThan,
        LessThan,
        Equals,
    };

    struct OperationTask {
        Operation operation = Operation::Literal;
        std::vector<uint64_t>  literals;

        uint64_t exec() {
            uint64_t result = 0;
            switch (operation) {
            case Operation::Sum:
                for (auto& i : literals)
                    result += i;
                break;
            case Operation::Product:
                result = 1;
                for (auto& i : literals)
                    result *= i;
                break;
            case Operation::Minimum:
                result = *std::min_element(literals.begin(), literals.end());
                break;
            case Operation::Maximum:
                result = *std::max_element(literals.begin(), literals.end());
                break;
            case Operation::GreaterThan:
                result = literals.at(0) > literals.at(1);
                break;
            case Operation::LessThan:
                result = literals.at(0) < literals.at(1);
                break;
            case Operation::Equals:
                result = literals.at(0) == literals.at(1);
                break;
            }

            return result;
        }
    };

    std::vector<uint8_t> read_byte_array(const char* filename)
    {
        std::vector<uint8_t> ret;
        std::ifstream in(filename);
        std::string s;

        while (in >> s) {
            for (std::size_t i = 0; i < s.size(); i += 2) {
                ret.push_back(static_cast<uint8_t>(strtoul(s.substr(i, 2).c_str(), 0, 16)));
            }
        }

        return ret;
    }

    bool parse_packet(BitReader& reader, Result &result, bool align=false)
    {
        uint64_t n = 0;
        // Version
        if (!reader.read_bits(3, &n))
            return false;

        result.part1 += n;

        // Type
        if (!reader.read_bits(3, &n))
            return false;

        Operation op = Operation(n);

        if (op == Operation::Literal) {

            // Handle literal value
            uint64_t value = 0;

            do {
                if (!reader.read_bits(5, &n))
                    break;
                value <<= 4;
                value |= n & 0b1111;
            } while ((n & 0b10000) == 0b10000);

            result.operationResult = value;
        } else {

            OperationTask task;
            task.operation = op;

            // Handle operator
            if (!reader.read_bits(1, &n))
                return false;

            if (n == 0) {
                if (!reader.read_bits(15, &n))
                    return false;

                auto bitcnt = reader.m_bitcnt;
                while (reader.m_bitcnt < (bitcnt + n)) {
                    if (!parse_packet(reader, result))
                        return false;

                    task.literals.push_back(result.operationResult);
                }
            }
            else if (n == 1) {

                if (!reader.read_bits(11, &n))
                    return false;

                // handle sub-packets
                for (int i = 0; i < n; i++) {
                    if (!parse_packet(reader, result))
                        return false;

                    task.literals.push_back(result.operationResult);
                }
            }

            result.operationResult = task.exec();

        }

        if (align)
            reader.align();
        return true;
    }

    uint64_t part1(const char* filename)
    {
        Result result;
        auto ba = read_byte_array(filename);
        BitReader reader(ba);
        while (reader.canRead() && parse_packet(reader, result, true));
        return result.part1;
    }

    uint64_t part2(const char* filename)
    {
        Result result;
        auto ba = read_byte_array(filename);
        BitReader reader(ba);
        while (reader.canRead() && parse_packet(reader, result, true));
        return result.operationResult;
    }
}

int main()
{
    std::cout << "Result (part1, demo) = " << part1("aoc16_sample.txt") << std::endl;
    std::cout << "Result (part1, real) = " << part1("aoc16.txt") << std::endl;
    std::cout << "Result (part2, demo) = " << part2("aoc16_sample.txt") << std::endl;
    std::cout << "Result (part2, real) = " << part2("aoc16.txt") << std::endl;
}