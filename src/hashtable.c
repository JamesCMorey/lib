#include <stdio.h>
#include <stdlib.h>
#include <linkedlist.h>
#include <short_types.h>
#include <hashtable.h>

u8 ht_del(hashtable_t *H, void *key, u32 keysize) {
	u64 hash = fnv_1a(key, keysize);
	hash = hash % H->size;

	return ll_del(ll_get(H->buckets[hash].ll, key), key);
}

void *ht_get(hashtable_t *H, void *key, u32 keysize) {
	u64 hash = fnv_1a(key, keysize);
	hash = hash % H->size;

	return ll_get(H->buckets[hash].ll, key)->value;
}

u8 ht_add(hashtable_t *H, void *key, u32 keysize, void *value) {
	u64 hash = fnv_1a(key, keysize);
	hash = hash % H->size;

	H->buckets[hash].ll = ll_add(H->buckets[hash].ll, key, value);
	return 0;
}

hashtable_t *ht_init(u32 size) {
	hashtable_t *H = malloc(sizeof(hashtable_t));
	H->size = size;

	/* Zeroed to meet empty linkedlist allocation requirement of head being NULL */
	/* TODO: Optimize this by making it so the array doesn't have to be zeroed */
	H->buckets = calloc(size, sizeof(struct ht_node));

	if (!H || !H->buckets) {
		return NULL;
	}

	return H;
}

void ht_free(hashtable_t *H) {
	for(u32 i = 0; i < H->size; i++) {
		ll_free(H->buckets[i].ll);
	}

	free(H->buckets);
	free(H);
}


/* fnv-1a hashing algorithm */
u64 fnv_1a(void *data, u32 size) {
	u64 hash = 0xcbf29ce484222325ULL;
	u64 fnv_prime = 0x100000001b3ULL;

	for(u32 i = 0; i < size; i++) {
		hash = hash ^ ((u8 *)data)[i];
		hash = hash * fnv_prime;
	}

	return hash;
}
