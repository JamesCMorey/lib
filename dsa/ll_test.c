#include "ll.h"
#include <assert.h>


struct entry {
    int key;
    int val;
};

/* Return: rv > 0 if k1 > k2, rv < 0 if k1 < k2, and rv == 0 if k1 == k2 */
int key_cmp(void *k1, void *k2) {
    /* e1 < e2 ? -1 : e1 > e2 */
    return *(int*)k1 < *(int*)k2 ? -1 : *(int*)k1 > *(int*)k2;
}

void *entry_key(void *entry) {
    return &((struct entry*)entry)->key;
}

void entry_free(void *entry);

int main() {

    ll_t L = ll_new(&key_cmp, &entry_key, NULL);
    return 0;
}
