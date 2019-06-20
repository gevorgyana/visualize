#include "library.h"
#include "b_tree.h"
#include <iostream>

using std::cout;

class Wrapper
{
public:

    Wrapper(B_tree<int> *src) : m_src(src)
    {
        cout << "initialized;\n";
    }

    void insert(int key)
    {
        cout << "insert;\n";
        cout.flush();
    }
    bool is_inside_of(int key){
        return true;
    }
    void remove(int key)
    {
        cout << "remove;\n";
        cout.flush();
    }

private:
    B_tree<int> *m_src;
};

// wrapping methods!!!

//Wrapper* wrapper;

bool is_inside_of(int key) {
    Wrapper wrapper(new B_tree<int>(2));
    return wrapper.is_inside_of(key);
}

void remove_key(int key)
{
    Wrapper wrapper(new B_tree<int>(2));
    wrapper.remove(key);
    return;
}

Wrapper* wr = nullptr;

void insert_key(int key)
{
    if (!wr)
    {
        wr = new Wrapper(new B_tree<int>(2));
        cout << "init";
        cout.flush();
    }
    wr->insert(key);
    return;
}