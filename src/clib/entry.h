#ifndef CLIB_ENTRY_H
#define CLIB_ENTRY_H

#ifdef __cplusplus
extern "C" {
#endif

    // btree specific

void btree_remove_key(int key);
void btree_insert_key(int key);
bool btree_is_inside_of(int key);
void btree_init(int expansion_factor);
void btree_release();

#ifdef __cplusplus
}
#endif

#endif //CLIB_ENTRY_H