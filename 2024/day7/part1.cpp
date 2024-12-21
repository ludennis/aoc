#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <vector>

bool ite_(const unsigned long int target, const std::vector<int>& nums, int index)
{
    if (index <= 0)
    {
        if (target == nums[0])
            return true;
        else
            return false;
    }

    if (target % nums[index] == 0)
        return ite_(target / nums[index], nums, index - 1) || ite_(target - nums[index], nums, index - 1);
    else
        return ite_(target - nums[index], nums, index - 1);
}

bool IsTrueEquation(const unsigned long int& target, const std::vector<int>& nums)
{
    return ite_(target, nums, nums.size() - 1);
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
            // std::cout << std::endl;

            std::cout << "target = " << n << ", nums = ";
            for (auto &i : nums)
                std::cout << i << ",";

            auto b = IsTrueEquation(n, nums);
            if (b)
                sum += n;
            std::cout << " ===> is true equation = " << b;

            std::cout << std::endl;
        }
    }

    std::cout << "Sum of true equations = " << sum << std::endl;
    return 0;
}