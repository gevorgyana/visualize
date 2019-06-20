#ifndef CLIB_LIBRARY_H
#define CLIB_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

void remove_key(int key);
void insert_key(int key);
bool is_inside_of(int key);
//void release_tree_mem();

#ifdef __cplusplus
}
#endif

#endif //CLIB_LIBRARY_H