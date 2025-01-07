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

void print_list(ll_t L) {
    printf("<HEAD>");
    ll_traverse(L, &proc, NULL);
    printf("---<TAIL>\n");
    puts("");
}

ll_t init_test() {
    ll_t L = ll_new(&key_cmp, &entry_key, &entry_free);
    assert(ll_size(L) == 0 && ll_empty(L));
    assert(L->key_cmp == &key_cmp);
    assert(L->entry_key == &entry_key);
    assert(L->entry_free == &entry_free);
    return L;
}

ll_t insertion_test() {
    ll_t L = init_test();

    puts("Inserting at head.");
    struct entry *t7 = entry_new(11, 2);
    ll_insert(L, t7);
    print_list(L);
    ll_free(L);

    L = init_test();
    puts("Inserting at tail.");
    struct entry *t8 = entry_new(11, 2);
    ll_insert_tail(L, t8);
    print_list(L);
    ll_free(L);

    L = init_test();

    puts("Inserting at index 0.");
    struct entry *t1 = entry_new(1, 1);
    ll_insert_at(L, t1, 0);
    print_list(L);

    puts("Inserting at head.");
    struct entry *t2 = entry_new(11, 2);
    ll_insert(L, t2);
    print_list(L);

    puts("Inserting at tail.");
    struct entry *t3 = entry_new(12, 3);
    ll_insert_tail(L, t3);
    print_list(L);

    puts("Inserting at head.");
    struct entry *t4 = entry_new(8, 4);
    ll_insert(L, t4);
    print_list(L);

    puts("Inserting at index 4.");
    struct entry *t5 = entry_new(4, 5);
    ll_insert_at(L, t5, 4);
    print_list(L);

    puts("Inserting at index 2.");
    struct entry *t6 = entry_new(20, 6);
    ll_insert_at(L, t6, 2);
    print_list(L);

    return L;
}

void deletion_test() {
    ll_t L = insertion_test();
    print_list(L);


    puts("Deleting index 2.");
    ll_del_at(L, 2);
    print_list(L);

    puts("Inserting at index 2.");
    struct entry *t1 = entry_new(20, 6);
    ll_insert_at(L, t1, 2);
    print_list(L);

    puts("Deleting head.");
    ll_del_head(L);
    print_list(L);

    puts("Deleting tail.");
    ll_del_tail(L);
    print_list(L);


    puts("Deleting entry with key 11.");
    int k = 11;
    ll_del(L, &k);
    print_list(L);

    puts("Deleting entry with key 12.");
    k = 12;
    ll_del(L, &k);
    print_list(L);

    puts("Inserting at index 2.");
    struct entry *t2 = entry_new(20, 7);
    ll_insert_at(L, t2, 2);
    print_list(L);

    puts("Deleting entry with key 20 from tail.");
    k = 20;
    ll_del_rev(L, &k);
    print_list(L);

    ll_free(L);
}

void print_entry(void *entry) {
    printf("<%d, %d>\n", ((struct entry*)entry)->key, ((struct entry*)entry)->val);
}

void get_test() {
    ll_t L = insertion_test();

    printf("Entry at index 0: ");
    print_entry(ll_at(L, 0));

    printf("Entry at index 1: ");
    print_entry(ll_at(L, 1));

    printf("Entry at index 5: ");
    print_entry(ll_at(L, 5));

    printf("Entry at index -1: ");
    print_entry(ll_at(L, -1));

    printf("Entry at index -3: ");
    print_entry(ll_at(L, -3));

    printf("Entry at index -6: ");
    print_entry(ll_at(L, -6));

    puts("");
}

int main() {
    puts("Init / free test");
    ll_free(init_test());
    puts("Insertion test");
    ll_free(insertion_test());
    puts("Deletion test");
    deletion_test();
    puts("get test");
    get_test();
    return 0;
}
