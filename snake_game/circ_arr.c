#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "deque.h"

#define MAX_SZ 100

typedef int data;
static int head, tail;
static data q[MAX_SZ];
static int size;


void init_q() {
	head = -1;
	tail = -1;
	size = 0;
}

bool is_empty() {
	return !size;
}

bool is_full() {
	return size == MAX_SZ;
}

void push_front(data val) {
	// first item inserted
	if (head < 0) {
		q[++head] = val;
		tail = 1;
	}
	if (!is_full()) {
		head = (head - 1) % MAX_SZ;
		q[head] = val;
	}
	size++;
}

void push_back(data val) {
	if (tail < 0) {
		q[tail++] = val;
		head++;
		tail++;
	}
	if (!is_full()) {
		
	}
}

char pop_front() {

}

char pop_back() {

}

int main () {


	return 0;
}