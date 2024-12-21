#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Blocks
{
    Blocks(int id, int file_size) : id_(id), file_size_(file_size), is_occupied_(false)
    {
        for (auto i = 0; i < file_size; ++i)
        {
            blocks_.push_back(id);
        }

        set_free_space(0);
    }

    void set_free_space(int free_space)
    {
        free_space_ = free_space;
        next_space_index_ = file_size_;

        for (auto i = 0; i < free_space; ++i)
        {
            blocks_.push_back(-1);
        }
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

    int get_free_space()
    {
        if (next_space_index_ > blocks_.size())
            return 0;
        return blocks_.size() - next_space_index_;
    }

    void allocate(int id, int file_size)
    {
        for (auto i = 0; i < file_size; ++i)
        {
            blocks_[next_space_index_] = id;
            next_space_index_++;
        }
    }

    int get_file_size()
    {
        return file_size_;
    }

    int get_id()
    {
        return id_;
    }

    void remove_file()
    {
        for (auto i = 0; i < file_size_; ++i)
            blocks_[i] = -1;
    }

    unsigned long long int checksum(unsigned long long int& index)
    {
        unsigned long long int sum = 0;
        for (auto& x : blocks_)
        {
            unsigned long long int xx = x;
            if (x != -1)
                sum += xx * index;
            index++;
        }

        return sum;
    }


    int id_;
    int file_size_;
    int free_space_;
    int next_space_index_;

    bool is_occupied_;

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

unsigned long long int get_checksum(std::shared_ptr<Blocks> p)
{
    unsigned long long int index = 0;
    unsigned long long int sum = 0;

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
        if (!right)
            break;
        left = head;

        while (left != tail || left != right)
        {
            if (!left)
                break;

            if (left->get_free_space() >= right->get_file_size())
            {
                left->allocate(right->get_id(), right->get_file_size());
                right->remove_file();
                break;
            }
            else
            {
                left = left->next;
                if (left == right)
                    break;
            }
        }
        right = right->prev;
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
            if (f.eof())
                break;
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

            if (f.eof())
                break;
            
            f >> c;
            int free_space = int(c - '0');

            if (f.eof())
                break;
            cur->set_free_space(free_space);
        }
    }

    print_list(head);

    align_memory_blocks(head, tail);

    print_list(head);

    std::cout << get_checksum(head) << std::endl;

    return 0;
}