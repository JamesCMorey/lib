#include <stdio.h>
#include <stdlib.h>
#include <linkedlist.h>
#include <short_types.h>


linkedlist_t *ll_get(struct ll_node *head, void *key) {
	struct ll_node *tmp = head;

	while(tmp != NULL && tmp->key != key) {
		tmp = tmp->next;
	}
	
	return tmp;
}

u8 ll_del(struct ll_node *head, void *key) {
	struct ll_node *tmp = (struct ll_node *)ll_get(head, key);

	if (tmp == NULL) {
		return -1;
	}

	tmp->prev->next = tmp->next;
	tmp->next->prev = tmp->prev;
	free(tmp);

	return 0;
}

linkedlist_t *ll_add(struct ll_node *head, void *key, void *value) {
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

void ll_free(linkedlist_t *head) {
	linkedlist_t *next;

	while(head != NULL) {
		next = head->next;
		free(head);
		head = next;
	}
}
