#ifndef B_TREE_B_TREE_H
#define B_TREE_B_TREE_H

// use node_key([shared_ptr<Node <T>>]) to get the node's key hash value
#define get_key(k, i) k -> m_keys[i] -> getKey()
#define get_cnt(k) k -> m_count
#define get_child(k, i) k -> m_children[i]

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <fstream>
#include <string>

template <typename T> class Node;

using std::pair;
using std::shared_ptr;
using std::make_shared;
using std::vector;

template <typename T>
using shared_node_t = shared_ptr<Node <T>>;

/*
class Book {
public:

    explicit Book(const unsigned &key) : m_key(key) {}

    unsigned getKey() const {
        return m_key;
    }

private:
    unsigned m_key;
};
*/

class Hashable {
public:

    explicit Hashable(const unsigned &key) : m_key(key) {}

    virtual unsigned getKey() const {
        return m_key;
    }

protected:
    unsigned m_key;
};

// Node вызывает само дерево извне оно невызывается запятые потерял
template <typename T>
struct Node {

    explicit Node (unsigned balance_factor)

    {
        m_is_leaf = true;
        m_count = 0;
        m_keys.resize(2 * balance_factor);
        m_children.resize(2 * balance_factor + 1);
    }

    vector <shared_ptr<Node<T>>> m_children;
    vector <shared_ptr<T>> m_keys;
    unsigned m_count;
    bool m_is_leaf;
};

template <typename T>
class B_tree {
public:

    explicit B_tree(unsigned balance_factor) : m_counter(0), m_balance_factor(balance_factor)
    {
        m_root = make_shared<Node<T>>(balance_factor);
    }

    //--- access operations

    pair <shared_node_t <T>, unsigned> search(T key) {
        return search(m_root, key);
    }

    // здесь get_cnt -  кол-во ключей
    pair <shared_node_t <T>, unsigned> search(shared_node_t <T> src, T key) {
        unsigned i = 1;
        while (i <= get_cnt(src) && key.getKey() > get_key(src, i)) {
            ++i;
        }
        if (i <= get_cnt(src) && key.getKey() == get_key(src,i)) {
            auto f = std::make_pair(src, i);
            return f;
        } else if (src -> m_is_leaf) {
            return std::make_pair(nullptr, 0);
        } else {
            return search(get_child(src, i), key);
        }
    }

    // TODO overload for integer key!


    // modification operations

    // ok
    void split(shared_node_t<T> src, unsigned i) {

        // закинуть в z что нужно
        shared_node_t <T> z = make_shared<Node <T>>(m_balance_factor);
        shared_node_t <T> y = src -> m_children[i];

        // do z
        z -> m_is_leaf = y -> m_is_leaf;
        get_cnt(z) = m_balance_factor - 1;
        for (int j = 1; j <= m_balance_factor - 1; j++) {
            //get_key(z, j) = get_key(y, j + m_balance_factor);
            z -> m_keys[j] = y -> m_keys[j + m_balance_factor];
        }
        if (!(y -> m_is_leaf)) {
            for (int j = 1; j <= m_balance_factor; j++) {
                get_child(z, j + 1)= get_child(y, j + m_balance_factor);
            }
        }

        // do y
        get_cnt(y) = m_balance_factor - 1;

        // insert median
        for (int j = src -> m_count + 1; j >= i + 1; --j) {
            get_child(src, j + 1) = get_child(src, j);
        }
        get_child(src, i + 1) = z;

        for (int j = src -> m_count; j >= i; --j) {
//            get_key(src, j + 1) = get_key(src, j);
            src -> m_keys[j + 1] = src -> m_keys[j];
        }
//        get_key(src, i) = get_key(y, m_balance_factor);
            src -> m_keys[i] = y -> m_keys[m_balance_factor];

        ++get_cnt(src);
    }

    void insert(T key) {
        insert(m_root, key);
    }

    void insert(shared_node_t <T> src, T key) {
        shared_node_t <T> r = m_root;
        if (get_cnt(r) == 2 * m_balance_factor - 1) {
            shared_node_t<T> s = make_shared<Node <T>>(m_balance_factor);
            m_root = s;
            s -> m_is_leaf = false;
            get_cnt(s) = 0;
            s -> m_children[1] = r;
            split(s, 1);
            insert_nonfull(s, key);
        }
        else {
            insert_nonfull(r, key);
        }
    }

    void remove (T key) {
        remove(m_root, key);
    }

    void remove (shared_node_t <T> src, T key) {
        if (find_key_in_node(src)) {
            unsigned pos = find_key_in_node(src);

            // leaf
            if (pos && src -> m_is_leaf) {
                unsigned i = pos;
                while (i <= get_cnt(src)) {
                    get_key(src, i) = get_key(src, i + 1);
                }
                --get_cnt(src);
            } else if (pos) {
                if (pos >= 2 && get_cnt(get_child(src, pos - 1)) >= m_balance_factor) {
                    shared_node_t <T> pred_p = get_child(src, pos - 1);
                    shared_node_t <T> pred = get_key(pred_p, get_cnt(pred_p));
                    remove(pred_p, pred);
                    get_key(src, pos) = pred;
                } else if (pos <= get_cnt(src) && get_cnt(get_child(src, pos + 1)) >= m_balance_factor) {
                    shared_node_t <T> succ_p = get_child(src, pos + 1);
                    shared_node_t <T> succ = get_key(succ_p, 1);
                    remove(succ_p, succ);
                    get_key(src, pos) = succ;
                } else if (pos >= 2 && pos <= get_cnt(src)
                            && (get_cnt(get_child(src, pos - 1)) == (get_cnt(get_child(src, pos + 1)))
                            && (get_cnt(get_child(src, pos + 1))  == m_balance_factor - 1)))
                {
                    shared_node_t <T> y = get_child(src, pos - 1);
                    shared_node_t <T> z = get_child(src, pos + 1);
                    shared_node_t <T> k = get_key(src, pos);
                    get_key(y, get_cnt(y) + 1) = k;

                    ++get_cnt(y);

                    get_child(y, get_cnt(y) + 1) = get_child(z, 1);

                    unsigned j = 1;
                    while (j <= get_cnt(z)) {
                        get_key(y, get_cnt(y) + j) = get_key(z, j);
                        get_child(y, get_cnt(y) + j + 1) = get_child(z, j + 1);
                        ++j;
                    }

                    get_cnt(y) += get_cnt(z);

                    // затереть src
                    j = pos;
                    while (j <= get_cnt(src) - 1) {
                        get_key(src, j) = get_key(src, j + 1);
                        ++j;
                    }

                    j = pos;
                    while (j <= get_cnt(src)) {
                        get_child(src, j) = get_child(src, j + 1);
                        ++j;
                    }
                }

            }
            else
            {
                unsigned i = 1;
                while (i <= get_cnt(src) && key > get_key(src, i)) {
                    ++i;
                }
                shared_node_t <T> c = get_child(src, i);
                if (get_cnt(c) == m_balance_factor - 1) {

                    // left
                    // todo check these functions!
                    //check?
                    if (i >= 2 && get_cnt(get_child(src, i - 1)) >= m_balance_factor) {
                        shared_node_t <T> c = get_child(src, pos);
                        shared_node_t <T> l = get_child(src, pos - 1);

                        // move c to the right
                        unsigned k = 1;
                        while (k <= get_cnt(c)) {
                            get_key(c, k + 1) = get_key(c, k);
                            ++k;
                        }
                        k = 1;
                        while (k <= get_cnt(c) + 1){
                            get_child(c, k + 1) = get_child(c, k);
                            ++k;
                        }

                        get_key(c, 1) = get_key(src, pos);
                        get_child(c, 1) = get_child(l, get_cnt(l) + 1);
                        get_key(src, pos) = get_key(l, get_cnt(l));
                        --(get_cnt(l));

                        remove(c, key);
                    }
                    //right todo check!!!!!!!!!! обнуляем j????
                    // check?
                    else if (i <= get_cnt(src) && get_cnt(get_child(src, i + 1)) >= m_balance_factor) {
                        shared_node_t <T> c = get_child(src, pos);
                        shared_node_t <T> r = get_child(src, pos + 1);
                        get_key(c, get_cnt(c) + 1) = get_key(src, pos);
                        get_child(c, get_cnt(c) + 2) = get_child(r, 1);
                        get_key(src, pos) = get_child(src, pos + 1);
                        ++get_cnt(c);
                        unsigned j = 1;
                        while (j <= get_cnt(r) - 1) {
                            get_key(r, j) = get_key(r, j + 1);
                            ++j;
                        }
                        j = 0;
                        while (j <= get_cnt(r)) {
                            get_child(r, j) = get_child(r, j + 1);
                            ++j;
                        }
                        --get_cnt(r);
                        remove(c, key);
                    }

                }
                if (i >= 2 && i <= get_cnt(src) &&
                    get_cnt(get_child(src, i - 1)) == m_balance_factor - 1 &&
                    get_cnt(get_child(src, i + 1)) == m_balance_factor - 1 &&
                    get_cnt(get_child(src, i)) == m_balance_factor - 1)
                {
                    //shared_node_t <T> l = get_child(src, i - 1);
                    shared_node_t <T> r = get_child(src, i + 1);
                    shared_node_t <T> c = get_child(src, i);
                    get_key(c, get_cnt(c) + 1) = get_key(src, pos);
                    ++(get_cnt(c));

                    unsigned k = 1;
                    while (k <= get_cnt(r))
                    {
                        get_key(c, get_cnt(c) + k) = get_key(r, k);
                        get_child(c, get_cnt(c) + k) = get_child(r, k);
                        ++k;
                    }
                    get_child(c, get_cnt(c) + get_cnt(r) + 1) = get_child(r, get_cnt(r) + 1);
                    get_cnt(c) += get_cnt(r) + 1;

                    k = pos;
                    while (k <= get_cnt(src) - 1) {
                        get_key(src, k) = get_key(src, k + 1);
                        ++k;
                    }

                    k = pos;
                    while (k <= get_cnt(src) - 1) {
                        get_child(src, k + 1) = get_child(src, k + 2);
                    }

                    remove(c, key);
                }
            }
        }




    }

    void stackWrite() {
        std::ofstream dotFile;
        dotFile.open ("./gr.dot");
        dotFile << "digraph {\n";
        //dotFile << "\t" << "a" << " [fillcolor=red style=filled];";

        write(dotFile, m_root, nullptr);

        dotFile << "}\n";
        dotFile.close();

        system("./show_png.sh");
    }


// m_child_count должен быть keys_count

private:

    // пр балансе надо смотреть не на m_counter а на кол-во узлов внутри !
    shared_node_t <T> m_root;
    unsigned m_counter;
    unsigned m_balance_factor;

    // --- b-tree specific helping operations
    void insert_nonfull(shared_node_t <T> src, T key) {
        unsigned i = get_cnt(src);
        if (src -> m_is_leaf) {
            while (i >= 1 && key.getKey() < get_key(src, i)) {
                src -> m_keys[i + 1] = src -> m_keys[i];
                --i;
            }
            src -> m_keys[i + 1] = make_shared<T>(key);
            ++(src -> m_count);
        } else {
            while (i >= 1 && key.getKey() < get_key(src, i)) {
                --i;
            }
            ++i;
            if (get_cnt(src -> m_children[i]) == 2 * m_balance_factor - 1) {
                split(src, i);
                if (key.getKey() > get_key(src, i)) {
                    ++i;
                }
            }
            insert_nonfull(src -> m_children[i], key);
        }
    }

    unsigned find_key_in_node(shared_node_t <T> src, T key) {
        unsigned i;
        bool found = false;
        for (i = 1; i <= get_cnt(src); ++i) {
            if (get_key(src, i) == key.getkey()) {
                return i;
            }
        }
        return 0;
    }


    // get_cnt - возвращает N - -во ключей, а не N + 1
    // условие: так делать можно
    void shift_extend_left(shared_node_t <T> src, shared_node_t <T> extra, shared_node_t <T> extra_child) {

        //shift to the right
        unsigned i = 1;
        while (i <= get_cnt(src)) {
            get_key(src, i + 1) = get_key(src, i);
            src -> m_children[i + 1] = src -> m_children[i];
            ++i;
        }
        src -> m_children[get_cnt(src) + 2] = src -> m_children[get_cnt(src) + 1];

        //insert the extra node
        src -> m_children[1] = extra_child;
        get_key(src, 1) = extra;

        // increment # of keys
        ++(get_cnt(src));
    }

    void shift_extend_right(shared_node_t <T> src, shared_node_t <T> extra, shared_node_t <T> extra_child) {
        unsigned i = get_cnt(src);
        get_key(src, i + 1) = extra;
        src -> m_children[i + 2] = extra_child;
        ++(get_cnt(src));
    }

    void shift_remove(shared_node_t <T> src) {
        unsigned i = get_cnt(src);
        while (i >= 2) {
            get_key(src, i - 1) = get_key(src, i);
            src -> m_children[i - 1] = src -> m_children[i];
            --i;
        }
        src -> m_children[get_cnt(src)] = src -> m_children[get_cnt(src) + 1];
        --(get_cnt(src));
    }

    void write(std::ofstream &dotFile, shared_node_t <T> src, shared_node_t <T> p)
    {

        if (p)
        {
            dotFile << "\t" << "\"" << p.get() << "\"" <<  " -> child" << "n" << src.get() << "n" << ";\n";
        }

        for (int i = 1; i <= src -> m_count; i++)
        {
            dotFile << "\t" << "\"" << src.get() << "\" -> key" <<  src -> m_keys[i] -> getKey() << ";\n";
        }

        if (!(src -> m_is_leaf)) {

            for (int i = 1; i <= get_cnt(src) + 1; i++)
            {
                write(dotFile, get_child(src, i), src);
            }
        }
    }
};

#endif //B_TREE_B_TREE_H
