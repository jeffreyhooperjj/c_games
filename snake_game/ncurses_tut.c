#include <ncurses.h>

void hello_world_ex() {
	initscr();
	printw("Hello world");
	refresh();
	getch();
	endwin();
}

void example_2() {
	int ch;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	printw("type any char to see in bold\n");
	ch = getch();

	printw("THe key pressed is: ");
	attron(A_BOLD);
	printw("%c", ch);
	attroff(A_BOLD);

	refresh();

	getch();
	endwin();



}

/* WINDOW CREATION EXAMPLE */

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

void window_border_ex() {
	WINDOW* my_win;
	int startx, starty, width, height;
	int ch;

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	height = 3;
	width = 10;
	starty = (LINES - height);
	startx = (COLS - width);
	printw("press q to exit\n");
	refresh();

	my_win = create_newwin(height, width, starty, startx);

	while((ch = getch()) != 'q') {
		switch(ch) {
			case KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty, --startx);
				break;
			case KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty, ++startx);
				break;
			case KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty, startx);
				break;
			case KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty, startx);
				break;
		}
	}

	endwin();

}

int main() {

	window_border_ex();

	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);

	box(local_win, 0, 0);

	wrefresh(local_win);

	return local_win;
}


void destroy_win(WINDOW *local_win) {
	// to properly delete the window, need this long funct call

	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');


	wrefresh(local_win);
	delwin(local_win);
}












