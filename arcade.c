/* Filename: arcade.c
 * Author: Dre Harm
 * Date: 4/22/26
 * Description: This is the main menu for of the program, allowing the user to select from the provided games.
 */

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "tictactoe.h"
#include "blackjack.h"

#define MENU_ITEMS 3

int main(void) {
    char *choices[MENU_ITEMS];
    int highlight;
    int choice;
    int ch;
    int row, col;
    int startY;

    /* this can be added onto later if we need to, remember to change MENU_ITEMS to reflect changes */
    choices[0] = "Tic-Tac-Toe";
    choices[1] = "Blackjack";
    choices[2] = "Settings";

    highlight = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);

    while(1) {
        choice = -1;

        while (choice == -1) {
            clear();
            getmaxyx(stdscr, row, col);

            box(stdscr, 0, 0); /* draws the boarder */

            char *title = "=== Arcade Games ===";
            char *instructions = "Use arrow keys for navigation, 'Enter' to select, and 'q' to quit";
            mvprintw(2, (col - strlen(title)) / 2, "%s", title);
            mvprintw(4, (col - strlen(instructions)) / 2, "%s", instructions);

            startY = row / 2 - MENU_ITEMS / 2; /* calculates the starting y position for the menu items */

            int i;
            for (i = 0; i < MENU_ITEMS; i++) { /* prints the menu items */
                int x = (col - strlen(choices[i])) / 2;

                if (i == highlight) { 
                    attron(A_REVERSE); /* highlights the selected menu item */
                    mvprintw(startY + i, x, "%s", choices[i]); /* prints the menu item */
                    attroff(A_REVERSE); /* turns off the highlight for the next item */
                } else { /* prints the non-highlighted menu items */
                    mvprintw(startY + i, x, "%s", choices[i]);
                }
            }
            refresh();
            ch = getch();

            switch (ch) {
                case KEY_UP:
                    if (highlight > 0) {
                        highlight--;
                    }
                    break;
                case KEY_DOWN:
                    if (highlight < MENU_ITEMS - 1) {
                        highlight++;
                    }
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
            endwin();
            runTicTacToe();
            initscr();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            curs_set(0);
        } else if (choice == 1) {
            endwin();
            run_blackjack();
            initscr();
            cbreak();
            noecho();
            keypad(stdscr, 1);
            curs_set(0);
        } else {
            strcpy(buffer, "You selected Settings. This has not been implemented yet.");
            mvprintw(row / 2, (col - strlen(buffer)) / 2, "%s", buffer);
            mvprintw(row / 2 + 2, (col - 24) / 2, "Press any key to exit.");
            refresh();
            getch();
        }
    }

    endwin();
    return 0;
}
