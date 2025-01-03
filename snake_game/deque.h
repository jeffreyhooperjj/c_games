#ifndef DEQUE_H
#define DEQUE_H

#include "snake.h"

typedef SnakePos data;


// need to make a dll implementation of deque
typedef struct Node {
	data data;
	struct Node *prev;
	struct Node *next;
} Node;

Node* push_front(Node *head, data data);
Node* push_back(Node *head, data data);
Node* pop_front(Node **head);
Node* pop_back(Node **head);
Node* get_head(Node* head);
Node* get_tail(Node* head);
void print_dll(Node* head);

#endif