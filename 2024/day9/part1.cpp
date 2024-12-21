#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Blocks
{
    Blocks(int id, int file_size) : id_(id)
    {
        free_space_index_ = -1;

        blocks_ = std::vector<int>();
        for (auto i = 0; i < file_size; ++i)
            blocks_.push_back(id);

        next_block_index_ = blocks_.size() - 1;
    }

    void set_free_space(int free_space)
    {
        free_space_index_ = blocks_.size();

        for (auto i = 0; i < free_space; ++i)
            blocks_.push_back(-1);
    }

    void print()
    {
        for (auto& b : blocks_)
        {
            if (b == -1)
                std::cout << '.';
            else
                std::cout << b << " ";
        }
    }

    bool empty()
    {
        if (next_block_index_ < 0)
            return true;
        return false;
    }

    bool is_free()
    {
        if (free_space_index_ >= 0 && free_space_index_ < blocks_.size())
            return true;
        return false;
    }

    int pop()
    {
        if (empty())
            return 0;

        auto b = blocks_[next_block_index_];
        blocks_[next_block_index_] = -1;
        next_block_index_--;
        return b;
    }

    void add(int x)
    {
        if (!is_free())
            return;
        blocks_[free_space_index_++] = x;
    }

    unsigned long int checksum(int& index)
    {
        if (empty())
            return 0;

        unsigned long int sum = 0;
        for (auto& x : blocks_)
        {
            if (x != -1)
                sum += x * index++;
        }

        return sum;
    }

    int id_;
    int free_space_index_;
    int next_block_index_;

    std::vector<int> blocks_;

    std::shared_ptr<Blocks> next;
    std::shared_ptr<Blocks> prev;
};

void print_list(std::shared_ptr<Blocks> p)
{
    while (p)
    {
        p->print();
        std::cout << std::endl;
        p = p->next;
    }
    std::cout << std::endl;
}

unsigned long int get_checksum(std::shared_ptr<Blocks> p)
{
    int index = 0;
    unsigned long int sum = 0;

    while (p)
    {
        sum += p->checksum(index);

        p = p->next;
    }

    return sum;
}

void amb(const std::shared_ptr<Blocks>& head, const std::shared_ptr<Blocks>& tail,
        std::shared_ptr<Blocks> left, std::shared_ptr<Blocks> right)
{
    while (right != head)
    {
        if (right && !right->empty())
        {
            while (left && !left->is_free())
            {
                left = left->next;
            }
            
            if (!left || left == right)
                return;

            left->add(right->pop());
        }
        else
        {
            right = right->prev;
        }
    }    
}

void align_memory_blocks(std::shared_ptr<Blocks> head, std::shared_ptr<Blocks> tail)
{
    amb(head, tail, head, tail);
}

int main(int main, char **argv)
{
    std::ifstream f("input.txt");
    std::string line;

    std::shared_ptr<Blocks> head;
    std::shared_ptr<Blocks> tail;
    std::shared_ptr<Blocks> cur;

    int id = 0;
    char c;
    if (f.is_open())
    {
        while (!f.eof())
        {
            f >> c;
            int file_size = int(c - '0');

            if (!head)
            {
                head = std::make_shared<Blocks>(id++, file_size);
                cur = head;
            }
            else
            {
                cur->next = std::make_shared<Blocks>(id++, file_size);
                cur->next->prev = cur;
                cur = cur->next;
                tail = cur;
            }

            if (!f.eof())
            {
                f >> c;
                int free_space = int(c - '0');

                cur->set_free_space(free_space);
            }
        }
    }

    print_list(head);

    align_memory_blocks(head, tail);

    print_list(head);

    std::cout << get_checksum(head) << std::endl;

    return 0;
}