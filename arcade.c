#include <ncurses.h>
#include <string.h>
#include "blackjack.h"

#define MENU_ITEMS 3

int main(void) {
    char *choices[MENU_ITEMS];
    int highlight;
    int choice;
    int ch;
    int row, col;
    int start_y;

    /* this can be added onto later if we need to, remember to change MENU_ITEMS to reflect changes */
    choices[0] = "Tic-Tac-Toe";
    choices[1] = "Blackjack";
    choices[2] = "Settings";

    highlight = 0;
    choice = -1;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);

    while (choice == -1) {
        clear();
        getmaxyx(stdscr, row, col);

        box(stdscr, 0, 0); /* draws the boarder */

        char *title = "=== Arcade Games ===";
        char *instructions = "Use arrow keys for navigation, 'Enter' to select, and 'q' to quit";
        mvprintw(2, (col - strlen(title)) / 2, "%s", title);
        mvprintw(4, (col - strlen(instructions)) / 2, "%s", instructions);


        start_y = row / 2 - MENU_ITEMS / 2; /* calculates the starting y position for the menu items */

        int i;
        for (i = 0; i < MENU_ITEMS; i++) { /* prints the menu items */
            int x = (col - strlen(choices[i])) / 2;

            if (i == highlight) { 
                attron(A_REVERSE); /* highlights the selected menu item */
                mvprintw(start_y + i, x, "%s", choices[i]); /* prints the menu item */
                attroff(A_REVERSE); /* turns off the highlight for the next item */
            } else { /* prints the non-highlighted menu items */
                mvprintw(start_y + i, x, "%s", choices[i]);
            }
        }
        refresh();
        ch = getch();

        switch (ch) {
            case KEY_UP:
                if (highlight > 0)
                    highlight--;
                break;
            case KEY_DOWN:
                if (highlight < MENU_ITEMS - 1)
                    highlight++;
                break;
            case 10: /* docs list 2 different enter key values so I'm including both */
            case KEY_ENTER: 
                choice = highlight;
                break;
            case 'q': /* covering both options for 'q' */
            case 'Q':
                endwin();
                return 0;
        }
    }

    clear();
    box(stdscr, 0, 0);

    char buffer[64];

    /* eventually this will open and run the different game files */
    if (choice == 0) {
        strcpy(buffer, "You selected Tic-Tac-Toe.");
    } else if (choice == 1) {
        extern void run_blackjack();
        run_blackjack();
        return 0;
    } else {
        strcpy(buffer, "You selected Settings.");
    }
    mvprintw(row / 2, (col - strlen(buffer)) / 2, "%s", buffer);

    mvprintw(row / 2 + 2, (col - 24) / 2, "Press any key to exit...");
    refresh();
    getch();

    endwin();
    return 0;
}
