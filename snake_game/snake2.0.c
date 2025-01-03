#include <ncurses.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "snake.h"
#include "deque.h"

#define MAX_NUM_MAGICAL_SPACES 6

/* TODO:
	make random generator to make items appear
*/


typedef Node SnakePiece;
dir snake_dir = up;
int score = 0;

SnakePiece* lengthen_snake(SnakePiece *head, SnakePiece *tail) {
	return push_back(head, tail->data);
}
SnakePiece* move_tail_to_head(SnakePiece *old_head, SnakePiece *tail) {
	return push_front(old_head, tail->data);
}

SnakePiece* get_snake_tail(SnakePiece* head) {
	return get_tail(head);
}

SnakePiece* remove_snake_tail(SnakePiece** head) {
	return pop_back(head);
}

SnakePiece* get_snake_head(SnakePiece* head) {
	return get_head(head);
}

void update_space(Board* board, int x, int y, char c) {
	board->board[x][y] = c;
}

void make_blank(Board* board, int x, int y) {
	update_space(board, x, y, ' ');
}

// used to determine where to place additional snake tail
bool check_pos(Board* board, int x, int y) {
	bool within_x = (x > 0) && (x < board->rows);
	bool within_y = (y > 0) && (y < board->cols);

	if (within_x && within_y && board->board[x][y] == ' ') {
		return true;
	}
	return false;
}

// how to grow and shrink snake?
void update_snake_size(Board* board, SnakePiece** head) {
	// need to check direction of snake
	// add new 'O' to back of snake
	// check coordinates of current tail
	SnakePiece* curr_tail = get_snake_tail(*head);
	SnakePiece* new_tail = (SnakePiece*) malloc(sizeof(SnakePiece));
	int curr_x = curr_tail->data.x;
	int curr_y = curr_tail->data.y;

	switch(snake_dir) {
		case left:
			// check right pos, up, and down
			 if (check_pos(board, curr_x, curr_y+1)) {
				new_tail->data.x = curr_x;
				new_tail->data.y = curr_y+1; 
			} else if (check_pos(board, curr_x+1, curr_y)) {
				new_tail->data.x = curr_x+1;
				new_tail->data.y = curr_y;
			} else if (check_pos(board, curr_x, curr_y-1)) {
				new_tail->data.x = curr_x;
				new_tail->data.y = curr_y-1;
			}
			break;
		case right:
			// check left, up and down
			if (check_pos(board, curr_x, curr_y-1)) {
				new_tail->data.x = curr_x;
				new_tail->data.y = curr_y-1;
			} else if (check_pos(board, curr_x, curr_y+1)) {
				new_tail->data.x = curr_x;
				new_tail->data.y = curr_y+1;
			} else if (check_pos(board, curr_x-1, curr_y)) {
				new_tail->data.x = curr_x-1;
				new_tail->data.y = curr_y;
			}
			break;
		case up:
			// check down, left, right
			if (check_pos(board, curr_x-1, curr_y)) {
				new_tail->data.x = curr_x-1;
				new_tail->data.y = curr_y;
			} else if (check_pos(board, curr_x, curr_y-1)) {
				new_tail->data.x = curr_x;
				new_tail->data.y = curr_y-1;
			} else if (check_pos(board, curr_x+1, curr_y)) {
				new_tail->data.x = curr_x+1;
				new_tail->data.y = curr_y;
			}
			break;
		case down:
			// check up, left, right
			if (check_pos(board, curr_x+1, curr_y)) {
				new_tail->data.x = curr_x+1;
				new_tail->data.y = curr_y;
			} else if (check_pos(board, curr_x, curr_y+1)) {
				new_tail->data.x = curr_x;
				new_tail->data.y = curr_y+1;
			} else if (check_pos(board, curr_x-1, curr_y)) {
				new_tail->data.x = curr_x-1;
				new_tail->data.y = curr_y;
			} 
			break;
	}

	*head = lengthen_snake(*head, new_tail);
}

void shrink_snake(Board *board, SnakePiece **head) {
	score -= 5;
	SnakePiece *tail = get_snake_tail(*head);
	make_blank(board, tail->data.x, tail->data.y);
	*head = remove_snake_tail(head);
}

SnakePiece* update_head_pos(SnakePiece* tail, int x, int y) {
	tail->data.x = x;
	tail->data.y = y;
	return tail;
}

//
void update_board_spaces(Board *board) {
	// if less than MAX_MAGICAL_SPACES spaces filled, pick a random item to add to board at valid pos
	if (board->max_free_spaces == 0) {
		return;
	}

	char items[] = {'$', '*', '@'}; 

	int randx = rand() % board->rows;
	int randy = rand() % board->cols;
	int rand_item = rand() % 3;

	if (rand_item == 2) {
		board->at_count++;
	}

	if (board->at_count > 1 && rand_item == 2) {
		rand_item = rand() % 2;
	}

	if (check_pos(board, randx, randy)) {
		update_space(board, randx, randy, items[rand_item]);
	}

	board->max_free_spaces--;
}

void end_game(Board* board) {
	int xmid = board->rows/2;
	int ymid = board->cols/2;
	// refresh();
	move(0,0);
	initialize_board(board);
	
	move(ymid, xmid);
	printw("\t\t\tGAME OVER\n");
	printw("\t\t\tSCORE: %d\n", score);
	refresh();

	// write score to file for posterity
	// exit
	exit(1);
}

void found_fruit() {
	score += 1;

}

void pause_game() {
	while (true) {
		int ch = getch();
		if (ch == (int)' ') {
			break;
		}
	}
}


/*
	Check current space to see if it is a border (end game),
	a fruit, a magical fruit, or a bad thing
*/
void check_space(Board *board, SnakePiece** head, SnakePos snake_pos) {
	char space = board->board[snake_pos.x][snake_pos.y];

	switch(space) {
		case '#':
			// gameover
			end_game(board);
			break;
		case '*':
			// fruit
			// add one to score
			found_fruit();
			board->max_free_spaces++;
			break;
		case '$':
			// mega fruit
			// add 3 to score
			// add one to snake size
			score += 3;
			update_snake_size(board, head);
			board->max_free_spaces++;
			break;
		case '@':
			// subract 5 from score
			// subtract 1 from snake size
			shrink_snake(board, head);
			board->max_free_spaces++;
			board->at_count--;
			break;
	}
}

void place_entire_snake(Board* board, SnakePiece* head) {
	while(head) {
		update_space(board, head->data.x, head->data.y, 'O');
		head = head->next;
	}
}

void check_snake_head(Board* board, SnakePiece** head, SnakePos pos) {
	// should check space here
	check_space(board, head, pos);
}


void update_snake_pos(Board* board, SnakePiece** head) {
	// check dir moving
	// move "tail" to be new front
	// popback
	// pushfront
	SnakePiece* tail = get_snake_tail(*head);
	SnakePiece* prev_head = get_snake_head(*head);
	*head = remove_snake_tail(head);

	make_blank(board, tail->data.x, tail->data.y);
	switch(snake_dir) {
		case left:
			// take last snake part and move to head.x - 1
			tail = update_head_pos(tail, prev_head->data.x, prev_head->data.y-1);
			break;
		case right:
			// take last snake part and move to head.x + 1
			tail = update_head_pos(tail, prev_head->data.x, prev_head->data.y+1);
			break;
		case up:
			// take last snake part and move to head.y + 1
			tail = update_head_pos(tail, prev_head->data.x-1, prev_head->data.y);
			break;
		case down:
			// take last snake part and move to head.y - 1
			tail = update_head_pos(tail, prev_head->data.x+1, prev_head->data.y);
			break;
	}

	SnakePos pos;
	pos.x = tail->data.x;
	pos.y = tail->data.y;
	if (prev_head != tail) {
		*head = move_tail_to_head(prev_head, tail);
	}
	check_snake_head(board, head, pos);
	place_entire_snake(board, *head);

	return;

}

void select_move(int ch) {
	switch(ch) {
		case KEY_LEFT:
			snake_dir = left;
			break;
		case KEY_RIGHT:
			snake_dir = right;
			break;
		case KEY_UP:
			snake_dir = up;
			break;
		case KEY_DOWN:
			snake_dir = down;
			break;
		case (int) ' ':
			// pause game
			pause_game();
			break;
	}
}

void get_move() {
	int ch = getch();

	select_move(ch);
}



void move_head(Board *board, SnakePiece** head) {
	// check direction
	// update location
	// check current space
	get_move();
	update_snake_pos(board, head);
	

}


int main() {
	// code to get window size
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	srand(time(NULL));

	Board* game_board = make_board(w.ws_row-1, w.ws_col);

	SnakePos snake_pos;

	snake_pos = initialize_board(game_board);
	game_board->max_free_spaces = MAX_NUM_MAGICAL_SPACES;

	SnakePiece *snake_head = (SnakePiece*) malloc(sizeof(SnakePiece));
	snake_head->data = snake_pos;
	snake_head->next = NULL;

	while (game_board->max_free_spaces) {
		update_board_spaces(game_board);
	}

	// game_board->board[5][40] = '$';
	// game_board->board[7][40] = '$';
	// game_board->board[15][40] = '@';
	// game_board->board[10][40] = '$';


	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	// set delay for reading input to be TIMEOUT ms
	timeout(TIMEOUT);

	// display_board(game_board);
	bool gameover = false;
	int prev_move = KEY_UP;
	while (!gameover) {
		display_board(game_board);
		move_head(game_board, &snake_head);
		update_board_spaces(game_board);
	}
	

	endwin();

	free(game_board);

	return 0;
}

void print_score() {
	// reset cursor to beginning of display
	move(0,0);
	printw("Current Score: %d", score);
}

void display_board(Board *board) {
	print_score();
	// reset cursor to beginning of display
	move(1,0);
	for (int i = 0; i < board->rows; i++) {
		printw("%s", board->board[i]);
	}
	refresh();
}

void make_barrier(Board *board) {
	// need to make a border of #s around the game board
	// first row, last row, first column, last column
	// first row and last row
	for (int i = 0; i < board->rows; i += board->rows - 1) {
		for (int j = 0; j < board->cols; j++) {
			// first row and last row
			board->board[i][j] = '#';
		}
	}
	for (int i = 0; i < board->rows; i++) {
		for (int j = 0; j < board->cols; j += board->cols - 1){
			// first column and last column
			board->board[i][j] = '#';
		}
	}
}

// start snake at center of board
SnakePos place_snake(Board *board) {
	SnakePos snake_pos;
	snake_pos.x = board->rows/2;
	snake_pos.y = board->cols/2;
	board->board[snake_pos.x][snake_pos.y] = 'O';
	return snake_pos;
}

SnakePos initialize_board(Board *board) {
	for (int i = 0; i < board->rows; i++) {
		for (int j = 0; j < board->cols; j++) {
			board->board[i][j]  = ' ';
		}
	}

	make_barrier(board);
	return place_snake(board);
}

// for reference of initiliazing 2D arr ptrs
Board* make_board(int rows, int cols) {
	Board* board = (Board*) malloc(sizeof(Board));
	board->rows = rows;
	board->cols = cols;
	board->board = (char**) malloc(rows * sizeof(char*));

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			board->board[i] = (char*) malloc(cols * sizeof(char));
		}
	}
	return board;
}

int move_logic(Board *board, SnakePos *snake_pos, int ch) {
	switch(ch) {
		case KEY_LEFT:
			board->board[snake_pos->x][snake_pos->y] = ' ';
			snake_pos->y--;
			board->board[snake_pos->x][snake_pos->y] = 'O';
			snake_dir = left;
			break;
		case KEY_RIGHT:
			board->board[snake_pos->x][snake_pos->y] = ' ';
			snake_pos->y++;
			board->board[snake_pos->x][snake_pos->y] = 'O';
			snake_dir = right;
			break;
		case KEY_UP:
			board->board[snake_pos->x][snake_pos->y] = ' ';
			snake_pos->x--;
			board->board[snake_pos->x][snake_pos->y] = 'O';
			snake_dir = up;
			break;
		case KEY_DOWN:
			board->board[snake_pos->x][snake_pos->y] = ' ';
			snake_pos->x++;
			board->board[snake_pos->x][snake_pos->y] = 'O';
			snake_dir = down;
			break;
		case 'q':
			return 0;
	}
	return 1;
}

int move_snake(Board *board, SnakePos *snake_pos, int prev_move) {
	int ch = getch();
	int result;

	if (ch == ERR) {
		result = move_logic(board, snake_pos, prev_move);
	} else {
		result = move_logic(board, snake_pos, ch);
		prev_move = ch;
	} 

	return (result) ? prev_move : result;
}