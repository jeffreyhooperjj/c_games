#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define RED "\033[91m"
#define GREEN "\033[92m"
#define ORANGE "\033[93m"
#define BLUE "\033[94m"
#define RESET "\x1B[0m"
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define CLEAR_SCREEN printf("\033[0;0H\033[2J")


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


void show_game(Board board, char* prev_board) {
	CLEAR_SCREEN;
	for (int i = 0; i < board.rows; i++) {
		for (int j = 0; j < board.cols; j++){
			// write(STDOUT_FILENO, &board.board[i*board.cols + j], sizeof(char));
			if (prev_board[i*board.cols + j] != board.board[i*board.cols + j]) {
				printf("x:%d, y:%d", i, j);
				gotoxy(i, j);
				write(STDOUT_FILENO, " ", sizeof(char));
				fflush(stdout);
			}
			
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

// just random testing
void testing_moving_cursor() {
	CLEAR_SCREEN;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++){
			write(STDOUT_FILENO, " ", sizeof(char));
		}
	}
	gotoxy(10, 5);
	printf("CHEESE");
	sleep(4);
	gotoxy(10, 5);
	printf("W");
	sleep(4);
	gotoxy(10, 9);
	sleep(4);
	printf("Z");
	gotoxy(10, 5);
	printf("     ");
	sleep(4);
}

void make_move() {
	
}

int main(int argc, char* argv[]) {
	setvbuf(stdout, NULL, _IONBF, 0); 
	// code to get window size
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	// initialize game board
	Board game_board;
	// the -1 is to fit on the terminal display
	game_board.rows = 10;
	game_board.cols = 10;
	int board_size = game_board.rows * game_board.cols;
	game_board.board = (char *) malloc(board_size * sizeof(char));
	char *prev_board = (char *) malloc(board_size * sizeof(char));
	memset(prev_board, '0', board_size);

	// initalialize game
	int score = 0;
	bool gameover = false;
	// printf("%d", STDOUT_FILENO);
	initialize_board(&game_board);
	make_barrier(&game_board);
	show_game(game_board, prev_board);

	
	

	while (!gameover) {
		show_game(game_board, prev_board);
		usleep(100 * 1000);
		make_move();
		memcpy(prev_board, game_board.board, board_size * sizeof(char));
	}

	// game over
	// print score
	printf(RED "Hello world\n" RESET);
	printf(BLUE "Test\n");
}