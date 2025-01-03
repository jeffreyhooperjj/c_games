#ifndef SNAKE_H
#define SNAKE_H

// delay getch() timeout to TIMEOUT ms
#define TIMEOUT 100

typedef struct Board {
	char** board;
	int rows;
	int cols;
	int max_free_spaces;
	int at_count;
} Board;

typedef struct SnakePos {
	int x;
	int y;
} SnakePos;

typedef enum dir {
	up, 
	down, 
	left, 
	right
} dir;

void display_board(Board *board);
void make_barrier(Board *board);
SnakePos place_snake(Board *board);
SnakePos initialize_board(Board *board);
Board* make_board(int rows, int cols);

int move_logic(Board *board, SnakePos *snake_pos, int ch);
int move_snake(Board *board, SnakePos *snake_pos, int prev_move);

#endif