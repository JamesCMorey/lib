#include "ll.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct entry {
    int key;
    int val;
};

void *entry_new(int k, int v) {
    struct entry *tmp = malloc(sizeof(*tmp));
    tmp->key = k;
    tmp->val = v;

    return tmp;
}

/* Return: rv > 0 if k1 > k2, rv < 0 if k1 < k2, and rv == 0 if k1 == k2 */
int key_cmp(void *k1, void *k2) {
    /* e1 < e2 ? -1 : e1 > e2 */
    return *(int*)k1 < *(int*)k2 ? -1 : *(int*)k1 > *(int*)k2;
}

void *entry_key(void *entry) {
    return &((struct entry*)entry)->key;
}

void entry_free(void *entry) {
    free(entry);
}

enum ll_traversalAction proc(void *entry, void *context) {
    printf("---<%d, %d>", ((struct entry*)entry)->key, ((struct entry*)entry)->val);
}

int main() {
    ll_t L = ll_new(&key_cmp, &entry_key, &entry_free);
    assert(ll_valid(L));
    assert(ll_size(L) == 0 && ll_empty(L));
    assert(L->key_cmp == &key_cmp);
    assert(L->entry_key == &entry_key);
    assert(L->entry_free == &entry_free);


    struct entry *t1 = entry_new(1, 1);
    struct entry *t2 = entry_new(2, 2);
    struct entry *t3 = entry_new(3, 3);
    struct entry *t4 = entry_new(4, 4);
    struct entry *t5 = entry_new(5, 5);
    ll_insert_at(L, t1, 0);
    ll_insert(L, t2);
    ll_insert(L, t3);
    ll_insert(L, t4);
    ll_insert_at(L, t5, 0);

    printf("<HEAD>");
    ll_traverse(L, &proc, NULL);
    printf("---<TAIL>\n");

    int k = 2;
    ll_del(L, &k);

    printf("<HEAD>");
    ll_traverse(L, &proc, NULL);
    printf("---<TAIL>\n");

    printf("<TAIL>");
    ll_traverse_rev(L, &proc, NULL);
    printf("---<HEAD>\n");
    return 0;
}
