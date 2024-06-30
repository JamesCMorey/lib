#include <stdio.h>
#include <string.h>
#include <hashtable.h>
#include <pretty_print.h>

int main() {
	printf("%s[HASHMAP TEST]%s\n", BOLD, KNRM);

	hashtable_t *H = NULL;
	u8 code = 0;
	/* Keys */
	char *k1 = "a";
	char *k2 = "b";
	char *k3 = "c";
	char *k4 = "d";
	char *k5 = "e";
	char *c1 = "8yn0iYCKYHlIj4-BwPqk";
	char *c2 = "GReLUrM4wMqfg9yzV3KQ";
	
	/* Values */
	int v1 = 1;
	int v2 = 2;
	int v3 = 3;
	int v4 = 4;
	int v5 = 5;

	H = ht_init(10);

	if (H == NULL) { code = 1; }
	print_code("Hashtable allocation", code);
	code = 0;

	/* Proper FNV-1a Hashing (Through known hash) */
	if (0x4EAC0C95540867E4 != fnv_1a(c1, strlen(c1)) || fnv_1a(c2, strlen(c2)) != 0x4EAC0C95540867E4) { code = 1; }
	print_code("FNV-1a hashing algorithm", code);
	code = 0;


	/* Collision Resolution */
	ht_add(H, c1, strlen(c1), &v1);
	ht_add(H, c2, strlen(c2), &v2);

	if (*(int *)ht_get(H, c1, strlen(c1)) != v1 || *(int *)ht_get(H, c2, strlen(c2)) != v2) { code = 1; }
	print_code("Collision resolution", code);
	code = 0;

	/* TODO: Write other tests */

	/* Hashmap free */
	ht_free(H);

	return 0;
}
