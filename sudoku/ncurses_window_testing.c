#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>


#define BIG_BOX_SZ 3
#define SUDOKU_SZ 9

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int rows = w.ws_row;
	int cols = w.ws_col;

	int cell_row_sz = rows / BIG_BOX_SZ;
	int cell_col_sz = cols / BIG_BOX_SZ;

	printf("Cell size = %d x %d\n", cell_row_sz, cell_col_sz);

	printf("Window size: %d x %d\n", rows, cols);

	initscr();
	noecho();
	cbreak();

	WINDOW *big_windows[BIG_BOX_SZ * BIG_BOX_SZ];

	WINDOW *windows[SUDOKU_SZ * SUDOKU_SZ];

	// need 81 even rows and cols

	// 9 rows 9 cols

	// make 9 big boxes with 9 smaller boxes in each of them

	mousemask(ALL_MOUSE_EVENTS, NULL);

	int startx = 0;
	int starty = 0;
	// TODO: account for padding to center puzzle

	// initialize big boxes
	for (int row = 0; row < BIG_BOX_SZ; row++) {
		for (int col = 0; col < BIG_BOX_SZ; col++) {
			big_windows[col + col*row] = create_newwin(cell_row_sz, cell_col_sz, starty, startx);
			// update startx for new col
			startx += cell_col_sz;
		}
		// update starty for new row, set startx back to beginning of col
		startx = 0;
		starty += cell_row_sz;
	}

	// test adding char to window
	bool done = false;
	while(!done) {
		MEVENT event;
		int ch;
		ch = wgetch(big_windows[0]);
		if (ch == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				if (event.bstate & BUTTON1_CLICKED) {
					wmove(big_windows[0], 0,0);
					wprintw(big_windows[0], "HELLO");
					wrefresh(big_windows[0]);
				}
			}
		}
		// switch(ch) {
		// 	case BUTTON1_CLICKED:
		// 		wmove(big_windows[0], 5,5);
		// 		wrefresh(big_windows[0]);
		// 		break;
		// 	case BUTTON2_CLICKED:
		// 		done = true;
		// 		break;
		// }
	}

	// refresh();
	// move(0,0);
	wgetch(big_windows[0]);


	// getch();
	// endwin();

	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win) {	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}