#include "ds/uba.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void lifespan_test() {
    uba_t u = uba_new(10, 0, NULL);
    assert(uba_size(u) == 0);
    assert(uba_limit(u) == 10);

    uba_free(u);

    u = uba_new(0, 0, NULL);
    uba_free(u);
}

int main() {
    lifespan_test();


    return 0;
}
