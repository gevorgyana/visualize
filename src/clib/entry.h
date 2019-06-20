#ifndef CLIB_ENTRY_H
#define CLIB_ENTRY_H

#ifdef __cplusplus
extern "C" {
#endif

// insert
void btree_insert_key(int key);

// remove
void btree_remove_key(int key);

// is key there?
bool btree_is_inside_of(int key);

// init
void btree_init(int expansion_factor);

// release memory - same for all types of proxies
void btree_release();


#ifdef __cplusplus
}
#endif

#endif //CLIB_ENTRY_H