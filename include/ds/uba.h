#pragma once
#ifndef UBA_H
#define UBA_H

#include <stdbool.h>
#include <stddef.h>

/******************************************************************************/
/*                              Client Interface                              */
/******************************************************************************/

typedef void uba_entry_free_fn(void *);

/******************************************************************************/
/*                               Implementation                               */
/******************************************************************************/

typedef struct uba_Header *uba_t;

struct uba_Header {
    void **data;

    size_t size;
    size_t limit;

    bool raw;

    uba_entry_free_fn *entry_free;
};

/******************************************************************************/
/*                             Library Interface                              */
/******************************************************************************/

// typedef ______* uba_t;

/* Return an uba
 *
 * requires: 0 <= limit
 * ensures: rv != NULL
 * */
uba_t uba_new(size_t limit, bool raw, uba_entry_free_fn *entry_free);

/* Free U alongside entries if entry_free defined for U
 *
 * requires: U != NULL
 * */
void uba_free(uba_t U);


/* Return size of uba (used space)
 *
 * requires: U != NULL && !uba_raw(U)
 * ensures: U != NULL && 0 <= rv && rv < uba_limit(U)
 * */
size_t uba_size(uba_t U);

/* Return limit of uba (reserved space)
 *
 * requires: U != NULL
 * ensures: U != NULL && 0 < rv
 * */
size_t uba_limit(uba_t U);

/* Return boolean indicating whether uba is raw
 *
 * requires: U != NULL
 * ensures: U != NULL
 * */
bool uba_raw(uba_t U);

/* Return boolean indicating whether uba is empty
 *
 * requires: U != NULL && !uba_raw(U)
 * ensures: U != NULL && ((rv && !uba_size(U)) || (!rv && uba_size(U)))
 * */
bool uba_empty(uba_t U);

/******************************************************************************/
/*                             High-Level Access                              */
/******************************************************************************/

/* Return entry at index
 *
 * requires: U != NULL && 0 <= index
 *              && ((!uba_raw(U) && index < uba_size(U))
 *                || (uba_raw(U) && index < uba_limit(U)))
 * ensures: U != NULL
 * */
void *uba_get(uba_t U, size_t index);

/* Add entry to end of uba's used space
 *
 * requires: U != NULL && !uba_raw(U)
 * ensures: U != NULL
 * */
void uba_push(uba_t U, void *entry);

/* Delete last element of used space
 *
 * requires: U != NULL && !uba_raw(U)
 * ensures: U != NULL
 * */
void uba_pop(uba_t U);

/* Insert entry at index, moving current index to the right
 *
 * requires: U != NULL && !uba_raw(U) && 0 <= index && index <= uba_size(U)
 * ensures: U != NULL
 * */
void uba_insert(uba_t U, size_t index, void *entry);

/* Remove (and free) entry at index, moving higher-index entries to the left
 *
 * requires: U != null && !uba_raw(U) && uba_size(U) > 0
 *              0 <= index && index < uba_size(U)
 * ensures: U != NULL
 * */
void uba_remove(uba_t U, size_t index);

/* Update entry at index, freeing old entry *
 * requires: U != null && !uba_raw(U) && uba_size(U) > 0
 *              0 <= index && index < uba_size(U)
 * ensures: U != null
 * */
void uba_update(uba_t U, size_t index, void *entry);

/* Shrink reserved space to used space
 *
 * requires: U != null
 * ensures: U != null
 * */
void uba_shrink(uba_t U);

/******************************************************************************/
/*                              Low-Level Access                              */
/******************************************************************************/

/* Set index of uba to entry and resize if necessary
 *
 * requires: U != NULL && 0 <= index && index < limit
 * ensures: U != NULL
 * */
void uba_set(uba_t U, size_t index, void *entry);

/* Delete entry at index and shift all higher-index elements to the left
 *
 * requires: U != NULL && 0 <= index && index < limit
 * ensures: U != NULL
 * */
void uba_del(uba_t U, size_t index);

/* Return pointer to internal array
 *
 * requires: U != NULL
 * ensures: U != NULL && rv
 * */
void *uba_data(uba_t U);

/* Change limit to new_limit if new_limit > uba_size(U)
 *
 * requires: U != NULL && uba_size(U) < new_limit && new_limit <= ULONG_MAX / 2
 * ensures: U != NULL
 * */
void uba_resize(uba_t U, size_t new_limit);

#endif
