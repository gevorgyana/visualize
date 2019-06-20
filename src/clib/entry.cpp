#include "entry.h"
#include "b_tree.h"

#include <iostream>
#include <exception>
#include <string>

using std::string;
using std::cout;
using std::exception;
using std::endl;

// use custom exceptions
struct invalid_ptr : public exception {
    const char * what () const noexcept override {
        return "invalid ptr";
    }
};

// use custom logging, add \n
void log(string msg)
{
    cout << "===CLIB MESSAGE===" << endl
         << msg << endl;
}

void log(const char* msg)
{
    string str_mesg(msg);
    log(str_mesg);
}

// edit this class to specify the actual behavior of functions found in header
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

Wrapper* wr = nullptr;

// wrapping methods
void btree_init(int expansion_factor)
{
    wr = new Wrapper(new B_tree<int>(expansion_factor));
}

bool btree_is_inside_of(int key) {
    try {

        if (!wr) {
            throw invalid_ptr();
        }

        return wr->is_inside_of(key);
    } catch (invalid_ptr& e)
    {
        log(e.what());
    }
}

void btree_remove_key(int key)
{
    try {

        if (!wr) {
            throw invalid_ptr();
        }

        wr->remove(key);
    } catch (invalid_ptr& e)
    {
        log(e.what());
    }
}

void btree_insert_key(int key)
{
    try {

        if (!wr)
        {
            throw invalid_ptr();
        }

        wr->insert(key);
    } catch (invalid_ptr& e)
    {
        log(e.what());
    }
}