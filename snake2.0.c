#include <ncurses.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>


// delay getch() timeout to TIMEOUT ms
#define TIMEOUT 100




typedef struct Board {
	char** board;
	int rows;
	int cols;
} Board;

typedef struct SnakePos {
	int x;
	int y;
} SnakePos;

void display_board(Board *board) {
	move(0,0);
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
	// printw("Here");
	// printf("%c", board->board[middle]);
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
			

			// refresh();
			break;
		case KEY_RIGHT:
			board->board[snake_pos->x][snake_pos->y] = ' ';
			snake_pos->y++;
			board->board[snake_pos->x][snake_pos->y] = 'O';
			// refresh();
			break;
		case KEY_UP:
			board->board[snake_pos->x][snake_pos->y] = ' ';
			snake_pos->x--;
			board->board[snake_pos->x][snake_pos->y] = 'O';
			// refresh();
			break;
		case KEY_DOWN:
			board->board[snake_pos->x][snake_pos->y] = ' ';
			snake_pos->x++;
			board->board[snake_pos->x][snake_pos->y] = 'O';
			// refresh();
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

	// switch(ch) {
	// 	case KEY_LEFT:
	// 		board->board[snake_pos->x][snake_pos->y] = ' ';
	// 		snake_pos->y--;
	// 		board->board[snake_pos->x][snake_pos->y] = 'O';
			

	// 		// refresh();
	// 		break;
	// 	case KEY_RIGHT:
	// 		board->board[snake_pos->x][snake_pos->y] = ' ';
	// 		snake_pos->y++;
	// 		board->board[snake_pos->x][snake_pos->y] = 'O';
	// 		// refresh();
	// 		break;
	// 	case KEY_UP:
	// 		board->board[snake_pos->x][snake_pos->y] = ' ';
	// 		snake_pos->x--;
	// 		board->board[snake_pos->x][snake_pos->y] = 'O';
	// 		// refresh();
	// 		break;
	// 	case KEY_DOWN:
	// 		board->board[snake_pos->x][snake_pos->y] = ' ';
	// 		snake_pos->x++;
	// 		board->board[snake_pos->x][snake_pos->y] = 'O';
	// 		// refresh();
	// 		break;
	// 	case 'q':
	// 		return 0;
	// }
	// // move(0,0);
	// // printw("snake position: %d %d\n", snake_pos->x, snake_pos->y);

	// return 1;
}


int main() {
	// code to get window size
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	Board* game_board = make_board(w.ws_row, w.ws_col);

	SnakePos snake_pos;

	snake_pos = initialize_board(game_board);


	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	// set delay for reading input to be TIMEOUT ms
	timeout(TIMEOUT);

	// display_board(game_board);
	bool gameover = false;
	int prev_move = KEY_UP;
	do {
		display_board(game_board);
		// move_snake(game_board, &snake_pos);
		// refresh();
	} while ((prev_move = move_snake(game_board, &snake_pos, prev_move)));
	
	// printw("%s", game_board->board);
	// printw("%d", snake_pos);
	// refresh();
	// getch();
	endwin();

	free(game_board);

	return 0;


}