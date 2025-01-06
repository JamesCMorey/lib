#include "ll.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct ll_Header *ll_new(ll_key_cmp_fn *key_cmp,
                         ll_entry_key_fn *entry_key,
                         ll_entry_free_fn *entry_free) {
    assert(key_cmp && entry_key);

    struct ll_Header *L = malloc(sizeof(*L));
    L->head = malloc(sizeof(*L->head));
    L->tail = malloc(sizeof(*L->tail));

    L->head->next = L->tail;
    L->head->prev = NULL;

    L->tail->next = NULL;
    L->tail->prev = L->head;

    L->size = 0;

    L->key_cmp = key_cmp;
    L->entry_key = entry_key;
    L->key_cmp = key_cmp;

    assert(ll_valid(L));
    return L;
}

void ll_free(ll_t L) {
    assert(is_valid(L));
    struct ll_Node *curr = L->head->next;
    struct ll_Node *next_node = curr->next;

    while (curr != L->tail) {
        if (L->ll_entry_free_fn)
            L->ll_entry_free_fn(curr->entry);

        free(curr);
        curr = next_node;
    }

    free(L->head);
    free(L->tail);
}

bool ll_valid(struct ll_Header *L) {
    if (L == NULL || L->head == NULL || L->tail == NULL)
        return false;

    for (struct ll_Node *p = L->head; p; p = p->next) {
        /* Check the next node's previous pointer */
        if (p != L->tail && p->next != NULL && p->next->prev != p)
            return false;

        /* Check previous node's next pointer */
        if (p != L->head && p->prev->next != p)
            return false;
    }

    return true;
}

size_t ll_size(struct ll_Header *L) {
    assert(ll_valid(L));
    return L->size;
}

bool ll_empty(struct ll_Header *L) {
    assert(ll_valid(L));
    return !ll_size(L);
}

bool ll_valid_index(struct ll_Header *L, int index) {
    assert(ll_valid(L));
    return (0 <= index && index < ll_size(L))
           || (index < 0 && -index <= ll_size(L));
}
