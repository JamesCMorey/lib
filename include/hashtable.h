#pragma once

#include <short_types.h>
#include <linkedlist.h>

struct ht_node {
	u64 key;
	linkedlist_t *ll;
};

typedef struct hashtable_t {
	u32 size;
	struct ht_node *buckets;
} hashtable_t;

hashtable_t *ht_init(u32 size);
u8 ht_add(hashtable_t *H, void *key, u32 keysize, void *value);
u64 fnv_1a(void *data, u32 size);
void *ht_get(hashtable_t *H, void *key, u32 keysize);
u8 ht_del(hashtable_t *H, void *key, u32 keysize);
