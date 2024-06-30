#include <stdio.h>
#include <string.h>
#include <hashtable.h>

int main() {
	hashtable_t *H = NULL;

	H = ht_init(10);
	if (H == NULL) {
		printf("Unable to allocate hashtable");
	}
	char *test =  "test";
	char *asdf = "asdf";
	char *t1 = "8yn0iYCKYHlIj4-BwPqk";
	char *t2 = "GReLUrM4wMqfg9yzV3KQ";
	ht_add(H, test, strlen(test), t1);
	ht_add(H, asdf, strlen(test), t2);

	printf("%lu, %lu\n", fnv_1a(t1, strlen(t1)), fnv_1a(t2, strlen(t2)));

	printf("%s\n", (char *)ht_get(H, test, strlen(test)));
	printf("%s\n", (char *)ht_get(H, asdf, strlen(test)));

	ht_del(H, asdf, strlen(asdf));
	printf("%s\n", (char *)ht_get(H, asdf, strlen(test)));

	return 0;
}

