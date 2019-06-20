/*
 * THIS IS IMPLEMENTATION OF THE CLIB.
 * THIS LIBRARY CAN STORE OBJECT DIFFERENTIATED BY THEIR ID'S,
 * WHICH ARE SIMPLE UNSIGNED INTEGER VALUES. THE LIBRARY
 * IS CAPABLE OF MANAGING RESOURCES. THIS SIMPLY A DELEGATE TO
 * A SET OF TESTED AND RELIABLE CPP DATA STRUCTURES. IT IS NOT
 * RECOMMENDED CURRENTLY TO INSERT MULTIPLE INTS OF THE SAME VALUE
 * */

#include "entry.h"
#include "b_tree.h"

#include <iostream>
#include <exception>
#include <string>

//include set to fix temporary bug with removal
#include <set>

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

// use custom logging, adds newline character
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

// a bucket to store object ids, simply to interface with the tree
class Bucket : Hashable
{
public:
    explicit Bucket(const unsigned &key)
            : Hashable(key)
    {

    }

    unsigned getKey() const override
    {
        return m_key;
    }
};

// edit this class to specify the actual behavior of functions
class Wrapper
{
public:

    Wrapper(B_tree<Bucket> *src) : m_src(src)
    {
        log("The tree wrapper has been given a tree pointer");
    }

    void insert(int key)
    {
        log("The tree wrapper is about to handle insertion");
        //Bucket bucket(key);
        //m_src -> insert(bucket);
        m_set.insert(key);
    }

    bool is_inside_of(int key){
        log("The tree wrapper is about to handle membership check");
        //Bucket bucket(key);
        //auto info = m_src -> search(bucket);
        //return info.first != nullptr;
        auto pos = m_set.find(key);
        for (auto i : m_set)
        {
            cout << i << endl;
            if (i == key)
                return true;
        }
        return false;
    }

    void remove(int key)
    {

        log("The tree wrapper is about to handle removal");
        //Bucket bucket(key);
        //m_src -> remove(bucket);
        m_set.erase(m_set.find(key));
    }

private:
    B_tree<Bucket> *m_src;
    std::set <int> m_set;
};

// global variable for now
Wrapper* wr = nullptr;

// wrapping methods
void btree_init(int expansion_factor)
{
    wr = new Wrapper(new B_tree<Bucket>(expansion_factor));
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

void modify(int oper_type, int key)
{
    try {

        if (!wr) {
            throw invalid_ptr();
        }

        switch (oper_type)
        {
            case 0:
                wr -> insert(key);
                break;
            case 1:
                wr -> remove(key);
                break;
            default:
                log("Unable to determie what action to take");
                break;
        }
    } catch (invalid_ptr& e)
    {
        log(e.what());
    }
}


void btree_insert_key(int key)
{
    modify(0, key);
}

void btree_remove_key(int key)
{
    modify(1, key);
}