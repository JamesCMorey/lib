#include <stdlib.h>
#include <linkedlist.h>
#include <short_types.h>


void *ll_get(struct ll_node *head, u64 key) {
	struct ll_node *tmp = head;

	while(tmp != NULL && tmp->key != key) {
		tmp = tmp->next;
	}
	
	return tmp;
}

void ll_del(struct ll_node *head, u64 key) {
	struct ll_node *tmp = (struct ll_node *)ll_get(head, key);

	tmp->prev->next = tmp->next;
	tmp->next->prev = tmp->prev;
	free(tmp);

	return;
}

void *ll_add(struct ll_node *head, u64 key, void *value) {
	struct ll_node *tmp;
	tmp = malloc(sizeof(struct ll_node));

	tmp->key = key;	
	tmp->value = value;
	tmp->prev = NULL;

	if (head == NULL) {
		tmp->next = NULL;
		return tmp;
	}
	
	tmp->next = head;
	head->prev = tmp;

	return tmp;
}
