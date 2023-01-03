#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define RED "\033[91m"
#define GREEN "\033[92m"
#define ORANGE "\033[93m"
#define BLUE "\033[94m"
#define RESET "\x1B[0m"
#define CLEAR_SCREEN "\033[0;0H\033[2J"


typedef struct Board {
	char* board;
	int rows;
	int cols;
} Board;

/*
	Snake game
	makes a game with # as barriers, * as fruit, $ as megafruit, @ as bad thing
*/

// NOTE:
// accessing single arr as 2d arr 
// arr[i*M + j]


void show_game(Board board) {
	for (int i = 0; i < board.rows; i++) {
		for (int j = 0; j < board.cols; j++){
			printf("%c", board.board[i*board.cols + j]);
		}
	}
}

void initialize_board(Board *board) {
	for (int i = 0; i < board->rows; i++) {
		for (int j = 0; j < board->cols; j++) {
			board->board[i*board->cols + j]  = ' ';
		}
	}
}

void make_barrier(Board *board) {
	// need to make a border of #s around the game board
	// first row, last row, first column, last column
	// first row and last row
	for (int i = 0; i < board->rows; i += board->rows - 1) {
		for (int j = 0; j < board->cols; j++) {
			// first row and last row
			board->board[i*board->cols + j] = '#';
		}
	}
	for (int i = 0; i < board->rows; i++) {
		for (int j = 0; j < board->cols; j += board->cols - 1){
			// first column and last column
			board->board[i*board->cols + j] = '#';
		}
	}
}

void make_move() {
	exit(1);
}

int main(int argc, char* argv[]) {
	// code to get window size
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	// initialize game board
	Board game_board;
	// the -1 is to fit on the terminal display
	game_board.rows = w.ws_row - 1;
	game_board.cols = w.ws_col;
	game_board.board = (char *) malloc(game_board.rows * game_board.cols * sizeof(char));

	// initalialize game
	int score = 0;
	bool gameover = false;
	// printf("%d", STDOUT_FILENO);
	initialize_board(&game_board);
	make_barrier(&game_board);
	while (!gameover) {
		show_game(game_board);
		make_move();
	}

	// game over
	// print score
	printf(RED "Hello world\n" RESET);
	printf(BLUE "Test\n");
}