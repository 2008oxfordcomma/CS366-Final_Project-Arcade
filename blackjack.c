#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DECK_SIZE 52

typedef struct {
    int rank; /* 1–13 */
} Card;

Card deck[DECK_SIZE];
int deck_pos = 0;

const char *ranks[] = {
    "?", "A", "2", "3", "4", "5", "6",
    "7", "8", "9", "10", "J", "Q", "K"
};

/* Initialize deck (4 copies of each rank) */
void init_deck() {
    int i;
    for (i = 0; i < DECK_SIZE; i++) {
        deck[i].rank = (i % 13) + 1;
    }
}

/* Shuffle deck */
void shuffle_deck() {
    int i;
    for (i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
    deck_pos = 0;
}

/* Draw card */
Card draw_card() {
    return deck[deck_pos++];
}

/* Blackjack values */
int card_value(int rank) {
    if (rank > 10) return 10;
    if (rank == 1) return 11;
    return rank;
}

/* Hand total with ace adjustment */
int hand_total(Card hand[], int size) {
    int total = 0;
    int aces = 0;

    int i;
    for (i = 0; i < size; i++) {
        total += card_value(hand[i].rank);
        if (hand[i].rank == 1) aces++;
    }

    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }

    return total;
}

/* Print a single card */
void print_card(Card c) {
    printw("[%s]", ranks[c.rank]);
}

/* Print centered hand (USES SAME MENU ALGORITHM) */
void print_hand(int y, int col, char *label, Card hand[], int size, int hide_first) {
    char line[256] = "";
    char temp[32];

    strcat(line, label);
    strcat(line, ": ");

    int i;
    for (i = 0; i < size; i++) {
        if (i == 0 && hide_first) {
            strcat(line, "[??] ");
        } else {
            snprintf(temp, sizeof(temp), "[%s] ", ranks[hand[i].rank]);
            strcat(line, temp);
        }
    }

    mvprintw(y, (col - strlen(line)) / 2, "%s", line);
}

/* Main game */
void run_blackjack() {
    initscr(); /* starts ncurses mode */
    cbreak(); /* turns off line buffering (keys immedietly to program(h/s,y/n)) */
    noecho(); /* key presses are hidden from players view */
    keypad(stdscr, 1); /* enables special keys(otherwise weird codes) */
    curs_set(0); /* hides blinking cursor */

    srand(time(NULL));

    int play_again = 1;

    while (play_again) {
        int row, col;
        getmaxyx(stdscr, row, col);

        Card player[12], dealer[12];
        int p_size = 0, d_size = 0;

        init_deck();
        shuffle_deck();

        /* initial deal */
        player[p_size++] = draw_card();
        dealer[d_size++] = draw_card();
        player[p_size++] = draw_card();
        dealer[d_size++] = draw_card();

        int stand = 0;
        int running = 1;

        while (running) {
            clear();
            getmaxyx(stdscr, row, col);

            int start_y = row / 2 - 4;

            /* Dealer + player centered exactly like menu */
            print_hand(start_y, col, "Dealer", dealer, d_size, !stand);
            print_hand(start_y + 2, col, "Player", player, p_size, 0);

            int p_total = hand_total(player, p_size);

            char total[64];
            sprintf(total, "Player Total: %d", p_total);
            mvprintw(start_y + 4, (col - strlen(total)) / 2, "%s", total);

            if (!stand) {
                char prompt[] = "(h)it  (s)tand  (q)quit";
                mvprintw(start_y + 6, (col - strlen(prompt)) / 2, "%s", prompt);

                int ch = getch();
		/* If player wants to hit(h), then a card is drawn, if player then busts, 
		 * player automatically stands and is dealers turn 
		 */
                if (ch == 'h') {
                    player[p_size++] = draw_card();
                    if (hand_total(player, p_size) > 21) {
                        stand = 1;
                    }
                } else if (ch == 's') {
                    stand = 1;
                } else if (ch == 'q') {
                    endwin();
                    return;
                }
            } else {
		
                /* After the player is done with their decision or busts, the dealer plays */
                while (hand_total(dealer, d_size) < 17) {
                    dealer[d_size++] = draw_card();
                }

                clear();
                getmaxyx(stdscr, row, col);

                start_y = row / 2 - 4;

                print_hand(start_y, col, "Dealer", dealer, d_size, 0);
                print_hand(start_y + 2, col, "Player", player, p_size, 0);

                int d_total = hand_total(dealer, d_size);

                char result[64];
                sprintf(result, "Player: %d  Dealer: %d", p_total, d_total);
                mvprintw(start_y + 4, (col - strlen(result)) / 2, "%s", result);

                char msg[64];
		
		/* Gives output if player or dealer wins */
                if (p_total > 21) {
                    strcpy(msg, "You bust! Dealer wins.");
                } else if (d_total > 21 || p_total > d_total) {
                    strcpy(msg, "You win!");
                } else if (p_total < d_total) {
                    strcpy(msg, "Dealer wins.");
                } else {
                    strcpy(msg, "Push.");
                }

                mvprintw(start_y + 6, (col - strlen(msg)) / 2, "%s", msg);

                char again[] = "Play again? (y/n)";
                mvprintw(start_y + 8, (col - strlen(again)) / 2, "%s", again);
		
		/* Gets the character if player wants to play again or not.*/
                int ch;
                while (1) {
                    ch = getch();
                    if (ch == 'y' || ch == 'Y') {
                        play_again = 1;
                        break;
                    } else if (ch == 'n' || ch == 'N') {
                        play_again = 0;
                        break;
                    }
                }

                running = 0;
            }

            refresh();
        }
    }

    endwin();
}
