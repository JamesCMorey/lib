#include "ll.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/******************************************************************************/
/*                              Init & Teardown                               */
/******************************************************************************/

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
    L->entry_free = entry_free;

    assert(ll_valid(L));
    return L;
}

void ll_free(ll_t L) {
    assert(ll_valid(L));
    struct ll_Node *curr = L->head->next;
    struct ll_Node *next_node = curr->next;

    while (curr != L->tail) {
        if (L->entry_free)
            L->entry_free(curr->entry);

        free(curr);
        curr = next_node;
    }

    free(L->head);
    free(L->tail);
}

/******************************************************************************/
/*                                 Accessors                                  */
/******************************************************************************/

struct ll_Node *ll_find_node(struct ll_Header *L, void *key) {
    assert(ll_valid(L));

    struct ll_Node *curr = L->head->next;

    while (curr != L->tail) {
        if (L->key_cmp(key, L->entry_key(curr->entry)) == 0) {
            return curr;
        }
    }

    return NULL;
}

void *ll_get(ll_t L, void *key) {
    assert(ll_valid(L));

    struct ll_Node *tmp = ll_find_node(L, key);

    return tmp ? tmp->entry : NULL;
}

void *ll_at(ll_t L, int index) {
    assert(ll_valid(L) && ll_valid_index(L, index));

    struct ll_Node *curr = L->head->next;
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }

    return curr->entry;
}

/******************************************************************************/
/*                                  Mutators                                  */
/******************************************************************************/

void ll_insert(ll_t L, void *entry) {
    struct ll_Node *tmp = malloc(sizeof(*tmp));
    tmp->entry = entry;

    tmp->next = L->head->next;
    tmp->prev = L->head;

    tmp->prev->next = tmp;
    tmp->next->prev = tmp;
}

/******************************************************************************/
/*                                 Traversal                                  */
/******************************************************************************/

void ll_traverse_opt(ll_t L, ll_proc_fn *p, void *context, bool rev) {
    assert(ll_valid(L) && p);

    struct ll_Node *tmp = rev ? L->tail->prev : L->head->next;
    enum ll_traversalAction rv;

    while (tmp != L->tail && tmp != L->head) {
        rv = p(tmp->entry, context);

        switch(rv) {
            case LL_TRAVERSAL_CONTINUE:
                break;

            case LL_TRAVERSAL_STOP:
                return;

            case LL_TRAVERSAL_DELETE: /* TODO: Implement */
                break;
        }
        tmp = rev ? tmp->prev : tmp->next;
    }
}

void ll_traverse(ll_t L, ll_proc_fn *p, void *context) {
    assert(ll_valid(L) && p);
    ll_traverse_opt(L, p, context, false);
}

void ll_traverse_rev(ll_t L, ll_proc_fn *p, void *context) {
    assert(ll_valid(L) && p);
    ll_traverse_opt(L, p, context, true);
}

/******************************************************************************/
/*                                    Info                                    */
/******************************************************************************/

size_t ll_size(struct ll_Header *L) {
    assert(ll_valid(L));
    return L->size;
}

bool ll_empty(struct ll_Header *L) {
    assert(ll_valid(L));
    return !ll_size(L);
}

/******************************************************************************/
/*                           Invariants/Validators                            */
/******************************************************************************/

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

bool ll_valid_index(struct ll_Header *L, int index) {
    assert(ll_valid(L));
    return (0 <= index && index < ll_size(L))
           || (index < 0 && -index <= ll_size(L));
}

/******************************************************************************/
/*                                 Debugging                                  */
/******************************************************************************/

void ll_print(struct ll_Header *L) {
    printf("<HEAD>");

    struct ll_Node *curr = L->head->next;
    while (curr != L->tail) {
        printf("-<???>");
        curr = curr->next;
    }

    printf("-<TAIL>\n");
}

