#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <vector>

bool ite_(const unsigned long int target, const std::vector<int>& nums, int index, unsigned long int sum)
{
    if (index >= nums.size())
    {
        if (target == sum)
            return true;
        else
            return false;
    }

    return ite_(target, nums, index + 1, sum + nums[index]) ||
            ite_(target, nums, index + 1, sum * nums[index]);
}

bool IsTrueEquation(const unsigned long int target, const std::vector<int>& nums)
{
    return ite_(target, nums, 0, 0);
}

int Power(int base, int exp)
{
    int result = base;
    for (auto i = 1; i < exp; ++i)
        result *= base;

    return result;
}

int NumOfDigits(int i)
{
    int digit = 0;
    while (i > 0)
    {
        i = i / 10;
        digit++;
    }

    return digit;
}

bool itte_(const unsigned long int target, const std::vector<int>& nums, int index, unsigned long int sum)
{
    if (index >= nums.size())
    {
        if (target == sum)
            return true;
        else
            return false;
    }

    return itte_(target, nums, index + 1, sum + nums[index]) ||
            itte_(target, nums, index + 1, sum * nums[index]) ||
            itte_(target, nums, index + 1, sum * Power(10, NumOfDigits(nums[index])) + nums[index]);
}

bool IsTrueTrueEquation(const unsigned long int target, const std::vector<int>& nums)
{
    return itte_(target, nums, 0, 0);
}

int main(int argc, char** argv)
{
    std::ifstream f("input.txt");
    std::string line;

    unsigned long int n;
    unsigned long int sum = 0;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            auto colon_index = line.find(":");

            n = std::stoul(line.substr(0, colon_index));

            std::stringstream ss(line.substr(colon_index + 1));

            int i;
            std::vector<int> nums;
            while (!ss.eof())
            {
                ss >> i;
                nums.emplace_back(i);
            }

            std::cout << "target = " << n << ", nums = ";
            for (auto &i : nums)
                std::cout << i << ",";

            auto b = IsTrueEquation(n, nums);
            if (b)
            {
                sum += n;
                std::cout << " => is true equation = " << b;
            }
            else
            {
                auto bb = IsTrueTrueEquation(n, nums);
                if (bb)
                    sum += n;

                std::cout << " ===> is true true equation = " << bb;

            }


            std::cout << std::endl;
        }
    }

    std::cout << "Sum of true equations = " << sum << std::endl;

    std::cout << NumOfDigits(123) << std::endl;
    std::cout << Power(10, 1) << std::endl;

    return 0;
}