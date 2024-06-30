#pragma once

#include "short_types.h"

struct ll_node {
	u64 key;
	void *value;
	struct ll_node *next;
	struct ll_node *prev;
};

/*
 * A doubly linked list that a u64 variable as a key and a void *  as a value.
 * */

/* Returns memory address of the first ll_node with the key being searched for. */
void *ll_get(struct ll_node *head, u64 key);

/* Deletes first node found with key. */
void ll_del(struct ll_node *head, u64 key);

/*
 * Takes the current head of the linked list and returns the new one. Stores key and
 * value in the new head.
 * */
void *ll_add(struct ll_node *head, u64 key, void *value);
