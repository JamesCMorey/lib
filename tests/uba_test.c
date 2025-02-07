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
    uba_t u = uba_new(10, 0, NULL);
    assert(uba_size(u) == 0);
    assert(uba_limit(u) == 10);

    uba_free(u);

    u = uba_new(0, 0, NULL);
    uba_free(u);
}


uba_t insertion_test() {
    uba_t u = uba_new(0, 0, &entry_free_fn);
    uba_push(u, mkint(5));
    uba_free(u);
}

int main() {
    lifespan_test();
    insertion_test();


    return 0;
}
