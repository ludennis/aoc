#include <fstream>
#include <iostream>


/**
  dial starts at 50, moves in range of 0, 99
  need to see how many times it touches 0
**/

int main(int argc, char** argv)
{
    std::ifstream file("input.txt");
    std::string line;
    auto count{0};
    auto dial{50};
    auto prev_dial{50};

    while (!file.eof())
    {
        char rotation;
        int dist;
        file >> rotation;
        file >> dist;

        std::cout << rotation << "," << dist << " ===> ";

        count += ( dist / 100 );
        dist %= 100;

        if (dist == 0)
            continue;

        switch (rotation)
        {
            case 'L':
                dial -= dist;
                break;
            case 'R':
                dial += dist;
                break;
            default:
                break;
        }

        if (dial > 100) count++;
        if (dial < 0 && prev_dial != 0) count++; // need prev_dial to check if dial moved from 0

        dial %= 100;

        if (dial < 0) dial += 100;

        if (dial % 100 == 0) count++;

        prev_dial = dial;

        std::cout << dial << ", count = " << count << std::endl;
    }

    std::cout << count << std::endl;

    return 0;
}

