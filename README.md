# Minimal C Library for Personal Projects

The goal of this project is to create an easy-to-use (for me) library of c utilities and data structures that can be referenced in other projects. This repo seeks to be as generic as possible and therefore is somewhat bare bones, with only the necessary functions being provided for data structures. I also didn't go out of my way to add features I wasn't planning to use.

## Installation
The following commands compiles the library files and copies the .a files alongside the header files to the appropriate system directories.

``` bash
git clone https://github.com/JamesCMorey/lib.git
cd lib && make
```

## Libs (eg: gcc main.c -lbit_reader)

### bit_reader

#### Functions

**get_max_64b(char \*dest, u64 value, size_t size)**

Store the first size bits of value in dest as 1s and 0s. Allows the user to get the binary representation of value in a human-readable format. 


### linkedlist
```c
typedef struct ll_node {
	void *key;
	void *value;
	struct ll_node *next;
	struct ll_node *prev;
} linkedlist_t;
```
#### Functions

**ll_get(linkedlist_t \*head, void \*key)**

// change this so the implementation isn't so exposed \
 ll_get returns a pointer to the first struct ll_node (linkedlist_t) that the searched-for key was found in.

**ll_add(linkedlist_t \*head, void \*key, void \*value)**
 
Inserts new ll_node with the key and value into the front of the list and returns the memory address of the new node (which is now the new head). To initialize a new list, just create a linkedlist_t ptr set to NULL and ll_add onto it.

```c
linkedlist_t *L = NULL;

char *key = "magic";
int value = 12;

L = ll_add(L, key, &value);
L = ll_add(L &value, key);
```

**ll_del(struct ll_node \*head, void \*key)**

Finds the first node in the list with the key and deletes it. Returns a byte sized exit code. 

### hashtable

This implementation uses chaining to solve hash collisions. This means it is possible to store more items in the hashtable than was specified during initialization as each index in the hashtable is a linked list.


```c 
struct ht_node {
	u64 key;
	linkedlist_t *ll;
};

typedef struct hashtable_t {
	u32 size;
	struct ht_node *buckets;
} hashtable_t;
```
#### Functions

**ht_init(u32 size)**

Allocates space for hashtable of size specified and returns pointer to it. Returns NULL on failure.

**ht_add(hashtable_t \*H, void \*key, u32 keysize, void \*value)**

Takes fnv-1a hash of the key and then stores a void pointer to the key as the key and stores a void pointer to the value alongside the key.


**fnv_1a(void \*data, u32 size)**

Takes fnv-1a hash of up to size bytes of data and returns it as a unsigned long.


**ht_get(hashtable_t \*H, void \*key, u32 keysize)**

Returns void pointer to the value stored with the key being searched for.

**ht_del(hashtable_t \*H, void \*key, u32 keysize)**

Searches for the key in the hashtable and deletes to corresponding entry. Returns exit code.

## Headers 

**(Each source file has an accompanying header not included below)**

### short_types.h

short_types simply redefines all the uint64_t typedefs as u64 and such to
create shorter, easier-to-type names.

## How to Uninstall
Move to the root of the repo and enter:

```bash
make uninstall
```