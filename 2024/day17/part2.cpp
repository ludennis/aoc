#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <limits>
#include <atomic>
#include <thread>
#include <memory>

// opcode 0
void adv(uint& a, const uint operand)
{
    a = a >> operand;
}

// opcode 1
void bxl(uint& b, const uint operand)
{
    // std::cout << "bxl: b = " << b << ", operand = " << operand << std::endl;
    b = b ^ operand;
}

// opcode 2
void bst(uint& b, const uint operand)
{
    b = operand % 8;
}

// opcode 3
void jnz(const uint& a, uint operand, size_t& p)
{
    if (a == 0)
        return;
    p = operand;
}

// opcode 4
void bxc(uint& b, const uint& c, uint operand)
{
    b = b ^ c;
}

// opcode 5
void out(const uint operand, std::stringstream& ss)
{
    ss << operand % 8 << ",";
}

// opcode 6
void bdv(const uint& a, uint& b, uint operand)
{
    b = a >> operand;
}

// opcode 7
void cdv(const uint& a, uint& c, uint operand)
{
    c = a >> operand;
}

void combo_operand(uint& a, uint& b, uint& c, uint& operand)
{
    if (operand == 4)
        operand = a;
    else if (operand == 5)
        operand = b;
    else if (operand == 6)
        operand = c;
}

std::vector<uint> run_program_until_halt(uint a, uint b, uint c, const std::vector<uint> program)
{
    std::vector<uint> result;
    size_t p = 0;
    while (p < program.size())
    {
        auto op = program[p];
        auto operand = program[p + 1];
        // std::cout << "op = " << op << ", operand = " << operand << std::endl;
        // std::cout << "Before op ==> a = " << a << ", b = " << b << ", c = " << c << std::endl;

        p += 2;

        switch (op)
        {
            case 0:
                combo_operand(a, b, c, operand);
                adv(a, operand);
                break;
            case 1:
                bxl(b, operand);
                break;
            case 2:
                combo_operand(a, b, c, operand);
                bst(b, operand);
                break;
            case 3:
                jnz(a, operand, p);
                break;
            case 4:
                bxc(b, c, operand);
                break;
            case 5:
                combo_operand(a, b, c, operand);
                result.push_back(operand % 8);
                for (auto i = 0u; i < result.size(); ++i)
                {
                    if (result[i] != program[i])
                        return result;
                }
                break;
            case 6:
                combo_operand(a, b, c, operand);
                bdv(a, b, operand);
                break;
            case 7:
                combo_operand(a, b, c, operand);
                cdv(a, c, operand);
                break;
        }

        // std::cout << "After op ==> a = " << a << ", b = " << b << ", c = " << c << std::endl;
        // std::cout << "output = " << ss.str() << std::endl;
    }

    return std::move(result);
}

int main(int argc, char **argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    uint a, b, c;
    std::vector<uint> program;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            if (auto found = line.find("Register A: ") != std::string::npos)
                a = std::stoi(line.substr(found + 11));
            else if (auto found = line.find("Register B: ") != std::string::npos)
                b = std::stoi(line.substr(found + 11));
            else if (auto found = line.find("Register C: ") != std::string::npos)
                c = std::stoi(line.substr(found + 11));
            else if (auto found = line.find("Program: ") != std::string::npos)
            {
                for (auto& c_ : line.substr(found + 8))
                {
                    if (c_ != ',')
                        program.emplace_back(uint(c_ - '0'));
                }
            }
        }
    }

    std::vector<std::unique_ptr<std::thread>> threads;
    std::atomic<size_t> cur_thread_num = 0;
    size_t max_thread_num = 100;
    std::atomic<bool> found_answer = false;

    a = 0;
    while (a < std::numeric_limits<uint>::max())
    {
        if (cur_thread_num >= max_thread_num)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (found_answer)
        {
            for (auto& t : threads)
                t->join();
            break;
        }

        threads.emplace_back(std::make_unique<std::thread>([a, b, c, program, &cur_thread_num, &found_answer]()
        {
            cur_thread_num++;
            auto result = run_program_until_halt(a, b, c, program);
            std::cout << "testing a = " << a << std::endl;

            if (result == program)
            {
                std::cout << "Program output copy of program with a = " << a << std::endl;
                found_answer = true;
            }
            cur_thread_num--;
        }));

        ++a;
    }

    for (auto& t : threads)
        t->join();

    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    return 0;
}