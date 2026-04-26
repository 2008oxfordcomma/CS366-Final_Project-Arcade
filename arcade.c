/* Filename: arcade.c
 * Author: Dre Harm
 * Date: 4/22/26
 * Description: This is the main menu for our program, allowing the user to select from the provided games and inturn run the one they select.
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

    /* common things to setup the ncurses enviroment */ 
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

            char *title = "~~~ CLI-cade ~~~";
            char *instructions = "Use the arrow keys for navigation, 'Enter' for selecting, and 'q' to quit";
            mvprintw(2, (col - strlen(title)) / 2, "%s", title);
            mvprintw(4, (col - strlen(instructions)) / 2, "%s", instructions);

            startY = row / 2 - MENU_ITEMS / 2; /* calculates the starting y position for the menu items */

            int i;
            for (i = 0; i < MENU_ITEMS; i++) { /* prints the menu items */
                int x = (col - strlen(choices[i])) / 2;

                if (i == highlight) { /* controls the highlighting of the menu items */
                    attron(A_REVERSE); 
                    mvprintw(startY + i, x, "%s", choices[i]);
                    attroff(A_REVERSE); 
                } else { /* then prints the unhighlighted items */
                    mvprintw(startY + i, x, "%s", choices[i]);
                }
            }
            refresh();
            ch = getch();

            switch (ch) { /* these keys are for navigateing the menu and controlling the 'flow' of the program */
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
