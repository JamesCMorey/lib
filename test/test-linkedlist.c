#include <stdio.h>
#include <linkedlist.h>

void print_node(struct ll_node *T) {
	printf("%d", *(int *)(T->value));
}

int main() {
	struct ll_node *T = NULL;
	int t1 = 12;

	T = ll_add(T, 1, &t1);
	printf("%p", T->value);
	//print_node(T);

	return 0;
}
