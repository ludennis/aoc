#include <fstream>
#include <iostream>
#include <string>

int GetNumberFromFile(std::ifstream& ifs)
{
    char c;
    std::string s;

    c = ifs.peek();

    while (c >= 48 && c <= 57)
    {
        s.push_back(c);
        ifs.get(c);
        c = ifs.peek();
    }

    return std::stoi(s);
}

int main(int argc, char** argv)
{
    std::ifstream file("input.txt");
    char c;
    std::string s;
    int a, b;
    int sum = 0;

    bool enabled = true;

    if (file.is_open())
    {
        while (!file.eof())
        {
            file.get(c);
            // file >> c;

            if (c == 'm')
            {
                file.get(c);
                if (c == 'u')
                {
                    file.get(c);
                    if (c == 'l')
                    {
                        file.get(c);
                        if (c == '(')
                        {
                            // mul(
                            c = file.peek();

                            if (c >= 48 && c <= 57)
                            {
                                a = GetNumberFromFile(file);

                                if (file.peek() == ',')
                                {
                                    file.get(c);
                                    c = file.peek();

                                    if (c >= 48 && c <= 57)
                                    {
                                        b = GetNumberFromFile(file);

                                        c = file.peek();
                                        if (c == ')')
                                        {
                                            if (enabled)
                                                sum += (a * b);
                                        }
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                    }
                }
            }
            else if (c == 'd')
            {
                file.get(c);
                if (c == 'o')
                {
                    file.get(c);
                    if (c == '(')
                    {
                        file.get(c);
                        if (c == ')')
                        {
                            enabled = true;
                        }
                    }
                    else if (c == 'n')
                    {
                        file.get(c);
                        if (c == '\'')
                        {
                            file.get(c);
                            if (c == 't')
                            {
                                file.get(c);
                                if (c == '(')
                                {
                                    file.get(c);
                                    if (c == ')')
                                    {
                                        enabled = false;
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                }
                                else
                                {
                                    continue;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }

    std::cout << "sum = " << sum << std::endl;
    return 0;
}