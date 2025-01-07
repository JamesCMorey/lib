#include "ll.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/******************************************************************************/
/*                                  INTERNAL                                  */
/******************************************************************************/
/*                               Helper Headers                               */
/******************************************************************************/

static struct ll_Node *ll_find_node(struct ll_Header *L, void *key, bool rev);
static void ll_traverse_opt(ll_t L, ll_proc_fn *p, void *context, bool rev);
static void ll_del_node(ll_t L, struct ll_Node *N);
static struct ll_Node *ll_node_at(ll_t L, int index);
static void ll_insert_node(ll_t L,
                    void *entry,
                    struct ll_Node *next,
                    struct ll_Node *prev);

/******************************************************************************/
/*                             Validation Headers                             */
/******************************************************************************/

static bool ll_valid(ll_t L);
static bool ll_valid_index(ll_t L, int index);

/******************************************************************************/
/*                                  EXTERNAL                                  */
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
        next_node = next_node->next;
    }

    free(L->head);
    free(L->tail);
    free(L);
}

/******************************************************************************/
/*                                 Accessors                                  */
/******************************************************************************/

void *ll_get(ll_t L, void *key) {
    assert(ll_valid(L));

    struct ll_Node *tmp = ll_find_node(L, key, false);
    return tmp ? tmp->entry : NULL;
}

void *ll_at(ll_t L, int index) {
    assert(ll_valid(L) && ll_valid_index(L, index));

    return ll_node_at(L, index)->entry;
}

/******************************************************************************/
/*                                  Mutators                                  */
/******************************************************************************/

void ll_insert(ll_t L, void *entry) {
    assert(ll_valid(L) && entry);

    ll_insert_node(L, entry, L->head->next, L->head);

    assert(ll_valid(L));
}

int ll_insert_tail(ll_t L, void *entry) {
    assert(ll_valid(L) && entry);

    ll_insert_node(L, entry, L->tail, L->tail->prev);

    assert(ll_valid(L));
}

int ll_insert_at(ll_t L,
                 void *entry,
                 int index) {
    assert(ll_valid(L) && ll_valid_index(L, index) && entry);

    struct ll_Node *tmp = ll_node_at(L, index);
    ll_insert_node(L, entry, tmp, tmp->prev);

    assert(ll_valid(L));
}

int ll_del(ll_t L, void *key) {
    assert(ll_valid(L) && !ll_empty(L));
    struct ll_Node *tmp = ll_find_node(L, key, false);

    if (tmp) {
        ll_del_node(L, tmp);

        assert(ll_valid(L));
        return 0;
    }

    assert(ll_valid(L));
    return 1;
}

int ll_del_rev(ll_t L, void *key) {
    assert(ll_valid(L) && !ll_empty(L));
    struct ll_Node *tmp = ll_find_node(L, key, true);

    if (tmp) {
        ll_del_node(L, tmp);

        assert(ll_valid(L));
        return 0;
    }

    assert(ll_valid(L));
    return 1;
}

int ll_del_head(ll_t L) {
    assert(ll_valid(L) && !ll_empty(L));

    ll_del_node(L, L->head->next);

    assert(ll_valid(L));
}

int ll_del_tail(ll_t L) {
    assert(ll_valid(L) && !ll_empty(L));

    ll_del_node(L, L->tail->prev);

    assert(ll_valid(L));

}

int ll_del_at(ll_t L, int index) {
    assert(ll_valid(L) && !ll_empty(L) && ll_valid_index(L, index));

    ll_del_node(L, ll_node_at(L, index));

    assert(ll_valid(L));
}

void *ll_update(ll_t L,
                void *key,
                void *new_entry,
                bool free_old) {
    assert(ll_valid(L) && !ll_empty(L) && new_entry);
    struct ll_Node *tmp = ll_find_node(L, key, false);

    if (free_old && L->entry_free)
        L->entry_free(tmp->entry);

    tmp->entry = new_entry;

    assert(ll_valid(L) && !ll_empty(L));
}

void *ll_update_at(ll_t L,
                   int index,
                   void *new_entry,
                   bool free_old) {
    assert(ll_valid(L) && ll_valid_index(L, index) && !ll_empty(L) && new_entry);

    struct ll_Node *tmp = ll_node_at(L, index);

    if (free_old && L->entry_free)
        L->entry_free(tmp->entry);

    tmp->entry = new_entry;

    assert(ll_valid(L) && !ll_empty(L));
}

/******************************************************************************/
/*                                 Traversal                                  */
/******************************************************************************/

void ll_traverse(ll_t L, ll_proc_fn *p, void *context) {
    assert(ll_valid(L) && p);
    ll_traverse_opt(L, p, context, false);
    assert(ll_valid(L));
}

void ll_traverse_rev(ll_t L, ll_proc_fn *p, void *context) {
    assert(ll_valid(L) && p);
    ll_traverse_opt(L, p, context, true);
    assert(ll_valid(L));
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
/*                                  INTERNAL                                  */
/******************************************************************************/
/*                           Invariants/Validators                            */
/******************************************************************************/

static bool ll_valid(struct ll_Header *L) {
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

static bool ll_valid_index(struct ll_Header *L, int index) {
    assert(ll_valid(L));
    return (0 <= index && index <= ll_size(L))
           || (index < 0 && -index <= ll_size(L));
}

/******************************************************************************/
/*                                  Helpers                                   */
/******************************************************************************/

static void ll_del_node(ll_t L, struct ll_Node *N) {
    assert(ll_valid(L) && N);

    N->next->prev = N->prev;
    N->prev->next = N->next;

    if (L->entry_free)
        L->entry_free(N->entry);

    free(N);
    L->size--;

    assert(ll_valid(L));
}

static void ll_traverse_opt(ll_t L, ll_proc_fn *p, void *context, bool rev) {
    assert(ll_valid(L) && p);

    struct ll_Node *tmp = rev ? L->tail->prev : L->head->next;
    enum ll_traversalAction rv;

    while (tmp != L->tail && tmp != L->head) {
        rv = p(tmp->entry, context);

        switch(rv) {
            case LL_TRAVERSAL_CONTINUE:
                break;

            case LL_TRAVERSAL_STOP:
                assert(ll_valid(L));
                return;

            case LL_TRAVERSAL_DELETE: /* TODO: Implement */
                break;
        }
        tmp = rev ? tmp->prev : tmp->next;
    }
    assert(ll_valid(L));
}

static struct ll_Node *ll_find_node(struct ll_Header *L, void *key, bool rev) {
    assert(ll_valid(L));

    struct ll_Node *curr = rev ? L->tail->prev : L->head->next;

    while (curr != L->tail && curr != L->head) {
        if (L->key_cmp(key, L->entry_key(curr->entry)) == 0) {
            assert(ll_valid(L));
            return curr;
        }
        curr = rev ? curr->prev : curr->next;
    }

    assert(ll_valid(L));
    return NULL;
}

struct ll_Node *ll_node_at(ll_t L, int index) {
    assert(ll_valid(L) && ll_valid_index(L, index));

    struct ll_Node *curr;
    if (index >= 0) {
        curr = L->head->next;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
    } else {
        curr = L->tail->prev;

        for (int i = -1; i > index; i--) {
            curr = curr->prev;
        }
    }

    return curr;
}

static void ll_insert_node(ll_t L,
                    void *entry,
                    struct ll_Node *next,
                    struct ll_Node *prev) {

    /* Create node and fill out info */
    struct ll_Node *tmp = malloc(sizeof(*tmp));
    tmp->entry = entry;
    tmp->next = next;
    tmp->prev = prev;

    /* Correct pointers in list to point at new node */
    tmp->prev->next = tmp;
    tmp->next->prev = tmp;
    L->size++;
}
