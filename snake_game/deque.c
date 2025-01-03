#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "deque.h"



/*
	methods needed:
	add at end
	add at beginning
	remove at end
*/

// int main() {

// 	SnakePos test;
// 	test.x = 12;
// 	test.y = 15;

// 	Node *head = NULL;
// 	Node *prev_head = NULL;
// 	head = push_front(head, test);
// 	// head = push_front(head, 12);
// 	// head = push_back(head, 15);

// 	// print_dll(head);
	
// 	printf("tail: %d\n", get_tail(head)->data.x);
// 	printf("tail: %d\n", get_tail(head)->data.y);
// 	// head = pop_back(&head);
// 	// printf("tail: %d\n", get_tail(head)->data);

// 	// print_dll(head);
// }

Node* push_front(Node *head, data data) {
	Node *new_node = (Node*) malloc(sizeof(Node));
	new_node->data = data;
	new_node->prev = NULL;
	if (!head) {
		head = new_node;
		head->prev = NULL;
		head->next = NULL;
	} else {
		new_node->next = head;
		head->prev = new_node;
		head = new_node;
	}
	return head;
}

Node* push_back(Node *head, data data) {
	Node *new_node = (Node*) malloc(sizeof(Node));
	new_node->next = NULL;
	new_node->data = data;
	Node *curr = head;

	while(curr && curr->next) {
		curr = curr->next;
	}

	curr->next = new_node;
	new_node->prev = curr->next;
	return head;
}

Node* pop_front(Node **head) {
	if (!(*head)) {
		return NULL;
	}
	Node *prev_head = *head;
	*head = (*head)->next;

	return prev_head;


}

Node* pop_back(Node **head) {
	if (!(*head)) {
		return NULL;
	}

	Node *curr = *head;
	while ((*head)->next && (*head)->next->next) {
		*head = (*head)->next;
	}
	Node *prev_tail = *head;
	(*head)->next = NULL;
	*head = NULL;

	return curr;

}

Node* get_head(Node* head) {
	if (!head) {
		return 0;
	}

	return head;
}

Node* get_tail(Node* head) {
	if (!head) {
		return NULL;
	}

	while (head && head->next) {
		head = head->next;
	}

	return head;
}

// void print_dll(Node* head) {
// 	Node *curr = head;
// 	while(curr) {
// 		printf("%d<-->", curr->data);
// 		curr = curr->next;
// 	}
// 	printf("NULL\n");
// }