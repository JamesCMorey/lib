#include "ds/uba.h"
#include <limits.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

/******************************************************************************/
/*                                  INTERNAL                                  */
/******************************************************************************/
/*                                  Helpers                                   */
/******************************************************************************/

static void uba_resize_auto(uba_t U) {
    assert(U != NULL);

    if (uba_size(U) >= uba_limit(U))
        uba_resize(U, uba_limit(U) * 2);
}

static bool uba_index_valid(uba_t U, size_t index) {
    assert(U != NULL);

    return 0 <= index
      && ((!uba_raw(U) && index <= uba_size(U))
        || (uba_raw(U) && index < uba_limit(U)));
}

/******************************************************************************/
/*                                  EXTERNAL                                  */
/******************************************************************************/
/*                              Init & Teardown                               */
/******************************************************************************/

uba_t uba_new(size_t limit, bool raw, uba_entry_free_fn *entry_free) {
    assert(0 <= limit);
    struct uba_Header *U = malloc(sizeof(*U));

    U->raw = raw;
    U->size = 0;
    U->limit = limit <= 0 ? 1 : limit;
    U->data = malloc(sizeof(void*) * U->limit);

    U->entry_free = entry_free;

    assert(U != NULL);
    return U;
}

void uba_free(uba_t U) {
    assert(U != NULL);
    if (U->entry_free && !uba_raw(U)) {
        for (size_t i = 0; i < uba_size(U); i++) {
            if (U->data[i])
                U->entry_free(U->data[i]);
        }
    }

    free(U->data);
    free(U);
}

/******************************************************************************/
/*                               Info Functions                               */
/******************************************************************************/

bool uba_raw(uba_t U) {
    assert(U != NULL);
    return U->raw;
}

size_t uba_size(uba_t U) {
    assert(U != NULL && !uba_raw(U));
    return U->size;
}

size_t uba_limit(uba_t U) {
    assert(U != NULL);
    return U->limit;
}

bool uba_empty(uba_t U) {
    assert(U != NULL && !uba_raw(U));
    return !uba_size(U);
}

/******************************************************************************/
/*                             High-Level Access                              */
/******************************************************************************/

void uba_resize(uba_t U, size_t new_limit) {
    assert(U != NULL && uba_size(U) < new_limit && new_limit <= ULONG_MAX / 2);

    U->limit = new_limit == 0 ? 1 : new_limit;
    void **arr = malloc(sizeof(void *) * new_limit);
    for (size_t i = 0; i < uba_size(U); i++) {
        arr[i] = U->data[i];
    }

    free(U->data);
    U->data = arr;
}

void uba_push(uba_t U, void *entry) {
    assert(U != NULL && !uba_raw(U));
    U->size++;
    uba_resize_auto(U);

    uba_set(U, uba_size(U) - 1, entry);
}

void uba_pop(uba_t U) {
    assert(U != NULL && !uba_raw(U));
    U->size--;

    if (U->entry_free)
        U->entry_free(U->data[U->size]);

    U->data[U->size] = NULL;
}

void uba_insert(uba_t U, size_t index, void *entry) {
    assert(U != NULL && !uba_raw(U) && 0 <= index && index <= uba_size(U));
    U->size++;
    uba_resize_auto(U);

    for(size_t i = uba_size(U); i > index; i--) {
        uba_set(U, i, uba_get(U, i - 1));
    }

    uba_set(U, index, entry);
}

void uba_remove(uba_t U, size_t index) {
    assert(U != NULL && !uba_raw(U) && uba_size(U) > 0
            && 0 <= index && index < uba_size(U));
    U->size--;

    if (U->entry_free)
        U->entry_free(U->data[index]);

    for(size_t i = uba_size(U) - 1; i > index; i--) {
        uba_set(U, i - 1, uba_get(U, i));
    }
}

void uba_update(uba_t U, size_t index, void *entry) {
    assert(U != NULL && !uba_raw(U) && 0 <= index && index < uba_size(U));

    if (U->entry_free)
        U->entry_free(U->data[index]);

    uba_set(U, index, entry);
}

void uba_shrink(uba_t U) {
    assert(U != NULL);
    uba_resize(U, uba_size(U) + 1);
}

/******************************************************************************/
/*                              Low-Level Access                              */
/******************************************************************************/

void *uba_get(uba_t U, size_t index) {
    assert(U != NULL && uba_index_valid(U, index)
            && ((!uba_raw(U) && index < uba_size(U))
              || (uba_raw(U) && index < uba_limit(U))));
    return U->data[index];
}

void uba_set(uba_t U, size_t index, void *entry) {
    assert(U != NULL && uba_index_valid(U, index));
    U->data[index] = entry;
}

void uba_del(uba_t U, size_t index) {
    assert(U != NULL && uba_index_valid(U, index));

    if (U->entry_free)
        U->entry_free(U->data[index]);

    U->data[index] = NULL;
}

void *uba_data(uba_t U) {
    assert(U != NULL);
    return U->data;
}
