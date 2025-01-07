#pragma once
#ifndef LL_H
#define LL_H

#include <stddef.h>
#include <stdbool.h>

/******************************************************************************/
/*                              Client Interface                              */
/******************************************************************************/

enum ll_traversalAction {
    LL_TRAVERSAL_CONTINUE,
    LL_TRAVERSAL_STOP,     /* Stop traversal */
    LL_TRAVERSAL_DELETE,   /* Delete the node/entry just processed */
};

/* Takes two keys and compares them
 *
 * ensures: (rv > 0 && k1 > k2) || (rv < 0 && k1 < k2) || (rv == 0 && k1 == k2)
 * */
typedef int ll_key_cmp_fn(void *k1, void *k2);

/* Takes a given entry and returns its key.
 *
 * requires: entry != NULL
 * */
typedef void *ll_entry_key_fn(void *entry);

/* Frees a given entry
 *
 * requires entry != NULL
 * */
typedef void ll_entry_free_fn(void *entry);

/* Run ll_proc_fn on a given entry with the given context when doing operations
 * like list traversal and return an action depending on the entry.
 *
 *
 * */
typedef enum ll_traversalAction ll_proc_fn(void *entry, void *context);

/******************************************************************************/
/*                               Implementation                               */
/******************************************************************************/

typedef struct ll_Header *ll_t;

struct ll_Node {
    void *entry;
    struct ll_Node *next;
    struct ll_Node *prev;
};

struct ll_Header {
    /* Points to dummy nodes */
    struct ll_Node *head;
    struct ll_Node *tail;

    size_t size;

    ll_key_cmp_fn *key_cmp;
    ll_entry_key_fn *entry_key;
    ll_entry_free_fn *entry_free;
};

/* ====== Validation ====== */
bool ll_valid(ll_t L);
bool ll_valid_index(ll_t L, int index);

/* ====== Helpers ====== */
struct ll_Node *ll_find_node(struct ll_Header *L, void *key, bool rev);
void ll_traverse_opt(ll_t L, ll_proc_fn *p, void *context, bool rev);
void ll_del_node(ll_t L, struct ll_Node *N);
struct ll_Node *ll_node_at(ll_t L, int index);

/******************************************************************************/
/*                             Library Interface                              */
/******************************************************************************/

//typedef struct ______ *ll_t;

/* ====== Init and Teardown ====== */

/* Initialize new linked list
 *
 * ensures: rv != NULL
 * */
ll_t ll_new(ll_key_cmp_fn *key_cmp,
            ll_entry_key_fn *entry_key,
            ll_entry_free_fn *entry_free);

/* Free linked list alongside entries if free_entries_fn is defined
 *
 * requires: L != NULL
 * */
void ll_free(ll_t L);

/* ====== Accessors ====== */

/* Returns entry with key or NULL if it doesn't exist
 *
 * requires: L != NULL
 * */
void *ll_get(ll_t L, void *key);

/* Returns entry at index
 *
 * requires: L != NULL && ((0 <= index && index < size(L))
 *                          || (index < 0 && -index <= size(L)))
 * */
void *ll_at(ll_t L, int index);

/* Returns size_t integer for amount of entries in list
 *
 * requires: L != NULL
 * ensures: 0 <= rv
 * */
size_t ll_size(ll_t L);

/* Returns int for empty or not
 *
 * requires: L != NULL
 * ensures: (rv == 1 && ll_size(L) == 0)
 *              || (rv == 0 && ll_size(L) > 0)
 * */
bool ll_empty(ll_t L);

/* Traverses L from head, calling p and passing each node and the context to p
 *
 * requires: L != NULL && p != NULL
 * */
void ll_traverse(ll_t L, ll_proc_fn *p, void *context);

/* Traverses L from tail, calling p and passing each node and the context to p
 *
 * requires: L != NULL && p != NULL
 * */
void ll_traverse_rev(ll_t L, ll_proc_fn *p, void *context);

/* ====== Mutators ====== */

/* Insert entry at head
 *
 * requires: L != NULL && entry != NULL
 * ensures: L != NULL && !ll_empty(L)
 * */
void ll_insert(ll_t L, void *entry);

/* Insert entry at tail
 *
 * requires: L != NULL && entry != NULL
 * ensures: L != NULL && !ll_empty(L)
 * */
int ll_insert_tail(ll_t L, void *entry);

/* Insert entry at index
 *
 * Inserts in front of node at index passed, thereby occupying the index.
 * Allows for negative indexing where -1 is the tail node.
 *
 * requires: L != NULL && entry!= NULL
 *              && ((0 <= index && index < size(L))
 *              || (index < 0 && abs(index) <= size(L)))
 * ensures: L != NULL && !ll_empty(L)
 * */
int ll_insert_at(ll_t L,
                 void *entry,
                 int index);

/* Searches from head and deletes first entry with key
 *
 * Inserts in front of node at index passed, thereby occupying the index.
 * Allows for negative indexing where -1 is the tail node.
 *
 * requires: L != NULL && !ll_empty(L)
 * ensures: L != NULL
 * */
int ll_del(ll_t L, void *key);

/* Searches from tail and deletes first entry with key
 *
 * requires: L != NULL && !ll_empty(L)
 * ensures: L != NULL
 * */
int ll_del_rev(ll_t L, void *key);

/* Delete entry at head
 *
 * requires: L != NULL && !ll_empty(L)
 * ensures: L != NULL
 * */
int ll_del_head(ll_t L);

/* Delete entry at tail
 *
 * requires: L != NULL && !ll_empty(L)
 * ensures: L != NULL
 * */
int ll_del_tail(ll_t L);

/* Delete entry at index
 *
 * Allows for negative indexing where -1 is the tail node.
 *
 * requires: L != NULL && !ll_empty(L)
 *              && ((0 <= index && index < ll_size(L))
 *              || (index < 0 && abs(index) <= ll_size(L)))
 * ensures: L != NULL
 * */
int ll_del_at(ll_t L, int index);

/* Find entry with key and replace it with new_entry, freeing the old entry if
 * the free_old flag is set. Returns old entry if free_old is not set.
 *
 * requires: L != NULL && new_entry != NULL
 *              && entry_key != NULL && key_cmp != NULL
 * */
void *ll_update(ll_t L,
                void *key,
                void *new_entry,
                bool free_old);

/* Find entry at index and replace it with new_entry, freeing the old entry if
 * the free_old flag is set. Returns old entry if free_old is not set.
 *
 * requires: L != NULL && new_entry != NULL
 *              && ((0 <= index && index < ll_size(L))
 *              || (index < 0 && abs(index) <= ll_size(L)))
 * */
void *ll_update_at(ll_t L,
                   int index,
                   void *new_entry,
                   bool free_old);

#endif
