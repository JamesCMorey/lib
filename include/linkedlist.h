#pragma once

#include "short_types.h"

typedef struct ll_node {
	void *key;
	void *value;
	struct ll_node *next;
	struct ll_node *prev;
} linkedlist_t;

/*
 * A doubly linked list that a u64 variable as a key and a void *  as a value.
 * */

/* Returns memory address of the first ll_node with the key being searched for. */
linkedlist_t *ll_get(struct ll_node *head, void *key);

/* Deletes first node found with key. */
u8 ll_del(struct ll_node *head, void *key);

/*
 * Takes the current head of the linked list and returns the new one. Stores key and
 * value in the new head.
 * */
linkedlist_t *ll_add(struct ll_node *head, void *key, void *value);

void ll_free(linkedlist_t *head);
