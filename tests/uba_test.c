#include "ds/uba.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void *mkint(int num) {
    void *tmp = malloc (sizeof(int));
    *(int*)tmp = num;
    return tmp;
}

void entry_free_fn(void *entry) {
    free(entry);
}

uba_t lifespan_test() {
    uba_t u = uba_new(10, false, NULL);
    assert(uba_size(u) == 0);
    assert(uba_limit(u) == 10);

    uba_free(u);

    u = uba_new(0, false, NULL);

    uba_free(u);
}

uba_t push_test() {
    uba_t u = uba_new(0, false, &entry_free_fn);

    uba_push(u, mkint(1));
    assert(*(int *)uba_get(u, 0) == 1);
    assert(uba_size(u) == 1);
    assert(uba_limit(u) == 2);

    uba_push(u, mkint(2));
    assert(*(int *)uba_get(u, 0) == 1);
    assert(*(int *)uba_get(u, 1) == 2);
    assert(uba_size(u) == 2);
    assert(uba_limit(u) == 4);

    uba_push(u, mkint(3));
    assert(*(int *)uba_get(u, 0) == 1);
    assert(*(int *)uba_get(u, 1) == 2);
    assert(*(int *)uba_get(u, 2) == 3);
    assert(uba_size(u) == 3);
    assert(uba_limit(u) == 4);

    uba_push(u, mkint(4));
    assert(*(int *)uba_get(u, 0) == 1);
    assert(*(int *)uba_get(u, 1) == 2);
    assert(*(int *)uba_get(u, 2) == 3);
    assert(*(int *)uba_get(u, 3) == 4);
    assert(uba_size(u) == 4);
    assert(uba_limit(u) == 8);

    return u;
}

void pop_test(uba_t u) {
    uba_pop(u);
    assert(*(int *)uba_get(u, 0) == 1);
    assert(*(int *)uba_get(u, 1) == 2);
    assert(*(int *)uba_get(u, 2) == 3);
    assert(uba_size(u) == 3);
    assert(uba_limit(u) == 8);

    uba_pop(u);
    assert(*(int *)uba_get(u, 0) == 1);
    assert(*(int *)uba_get(u, 1) == 2);
    assert(uba_size(u) == 2);
    assert(uba_limit(u) == 8);

    uba_pop(u);
    assert(*(int *)uba_get(u, 0) == 1);
    assert(uba_size(u) == 1);
    assert(uba_limit(u) == 8);

    uba_pop(u);
    assert(uba_size(u) == 0);
    assert(uba_limit(u) == 8);

    uba_free(u);
}

uba_t insert_test() {
    uba_t u = uba_new(0, false, &entry_free_fn);

    uba_insert(u, 0, mkint(11));
    assert(*(int *)uba_get(u, 0) == 11);
    assert(uba_size(u) == 1);
    assert(uba_limit(u) == 2);

    uba_insert(u, 0, mkint(12));
    assert(*(int *)uba_get(u, 0) == 12);
    assert(*(int *)uba_get(u, 1) == 11);
    assert(uba_size(u) == 2);
    assert(uba_limit(u) == 4);

    uba_insert(u, 1, mkint(13));
    assert(*(int *)uba_get(u, 0) == 12);
    assert(*(int *)uba_get(u, 1) == 13);
    assert(*(int *)uba_get(u, 2) == 11);
    assert(uba_size(u) == 3);
    assert(uba_limit(u) == 4);

    uba_insert(u, 3, mkint(15));
    assert(*(int *)uba_get(u, 0) == 12);
    assert(*(int *)uba_get(u, 1) == 13);
    assert(*(int *)uba_get(u, 2) == 11);
    assert(*(int *)uba_get(u, 3) == 15);
    assert(uba_size(u) == 4);
    assert(uba_limit(u) == 8);

    return u;
}

void remove_test(uba_t u) {

    uba_remove(u, 0);
    assert(*(int *)uba_get(u, 0) == 13);
    assert(*(int *)uba_get(u, 1) == 11);
    assert(*(int *)uba_get(u, 2) == 15);
    assert(uba_size(u) == 3);
    assert(uba_limit(u) == 8);

    uba_remove(u, 2);
    assert(*(int *)uba_get(u, 0) == 13);
    assert(*(int *)uba_get(u, 1) == 11);
    assert(uba_size(u) == 2);
    assert(uba_limit(u) == 8);

    uba_remove(u, 1);
    assert(*(int *)uba_get(u, 0) == 13);
    assert(uba_size(u) == 1);
    assert(uba_limit(u) == 8);

    uba_remove(u, 0);
    assert(uba_size(u) == 0);
    assert(uba_limit(u) == 8);

    uba_free(u);
}

void high_mutation_test() {
    pop_test(push_test());
    remove_test(insert_test());
}

void low_mutation_test() {
    return;
}

int main() {
    lifespan_test();
    high_mutation_test();
    low_mutation_test();

    return 0;
}
