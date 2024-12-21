#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>

uint a;
uint b;
uint c;

// opcode 0
void adv(uint operand)
{
    a = a >> operand;
}

// opcode 1
void bxl(uint operand)
{
    std::cout << "bxl: b = " << b << ", operand = " << operand << std::endl;
    b = b ^ operand;
}

// opcode 2
void bst(uint operand)
{
    b = operand % 8;
}

// opcode 3
void jnz(uint operand, size_t& p)
{
    if (a == 0)
        return;
    p = operand;
}

// opcode 4
void bxc(uint operand)
{
    b = b ^ c;
}

// opcode 5
void out(uint operand, std::stringstream& ss)
{
    ss << operand % 8 << ",";
}

// opcode 6
void bdv(uint operand)
{
    b = a >> operand;
}

// opcode 7
void cdv(uint operand)
{
    c = a >> operand;
}

void combo_operand(uint& operand)
{
    if (operand == 4)
        operand = a;
    else if (operand == 5)
        operand = b;
    else if (operand == 6)
        operand = c;
}

int main(int argc, char **argv)
{
    std::ifstream f(argv[1]);
    std::string line;

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

    size_t p = 0;
    uint op;
    uint operand;
    std::stringstream ss;
    while (p < program.size())
    {
        op = program[p];
        operand = program[p + 1];
        std::cout << "op = " << op << ", operand = " << operand << std::endl;
        std::cout << "Before op ==> a = " << a << ", b = " << b << ", c = " << c << std::endl;

        p += 2;

        switch (op)
        {
            case 0:
                combo_operand(operand);
                adv(operand);
                break;
            case 1:
                bxl(operand);
                break;
            case 2:
                combo_operand(operand);
                bst(operand);
                break;
            case 3:
                jnz(operand, p);
                break;
            case 4:
                bxc(operand);
                break;
            case 5:
                combo_operand(operand);
                out(operand, ss);
                break;
            case 6:
                combo_operand(operand);
                bdv(operand);
                break;
            case 7:
                combo_operand(operand);
                cdv(operand);
                break;
        }

        std::cout << "After op ==> a = " << a << ", b = " << b << ", c = " << c << std::endl;
        std::cout << "output = " << ss.str() << std::endl;
    }

    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    std::cout << "output = " << ss.str() << std::endl;
    return 0;
}