/* Filename: tictactoe.c
 * Author: Dre Harm
 * Date: 4/16/26
 * Description: 
 */

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define BOARD_SIZE 3
#define CELL_WIDTH 5
#define CELL_HEIGHT 3

static void drawBoard(int board[BOARD_SIZE][BOARD_SIZE], int rowOffset, int colOffset);
static int checkWinner(int board[BOARD_SIZE][BOARD_SIZE]);
static int isBoardFull(int board[BOARD_SIZE][BOARD_SIZE]);
static void drawCell(int row, int col, int value, int startY, int startX);
static void showWinnerMessage(const char *message, int row, int col);

int runTicTacToe(void) {
    int playAgain = 1;

    while(playAgain) {
        int board[BOARD_SIZE][BOARD_SIZE] = {0};
        int currentPlayer = 1;  /* 1 is for x and 2 is for O */
        int gameOver = 0;
        int winner = 0;
        int ch;
        int cursorRow = 0, cursorCol = 0;
        int row, col;
        int boardStartY, boardStartX;
        int boardWidth, boardHeight;
        int cursorScreenY, cursorScreenX;
        int i;
        char playerMsg[64];
        char *title = "Tic-Tac-Toe";
        char *instructions = "you can use the arrow keys to move, Space or Enter places your shape, and q quits";
        
        keypad(stdscr, TRUE);

        while (!gameOver) {
            clear();
            getmaxyx(stdscr, row, col);
            
            box(stdscr, 0, 0);
            
            mvprintw(2, (col - strlen(title)) / 2, "%s", title);
            
            mvprintw(4, (col - strlen(instructions)) / 2, "%s", instructions);
            
            sprintf(playerMsg, "Current Player: %c", (currentPlayer == 1) ? 'X' : 'O');
            mvprintw(6, (col - strlen(playerMsg)) / 2, "%s", playerMsg);
            
            /* calculates the board position so it's in the center */
            boardWidth = BOARD_SIZE * CELL_WIDTH;
            boardHeight = BOARD_SIZE * CELL_HEIGHT;
            boardStartY = (row - boardHeight) / 2;
            boardStartX = (col - boardWidth) / 2;
            
            drawBoard(board, boardStartY, boardStartX);
            
            cursorScreenY = boardStartY + (cursorRow * CELL_HEIGHT);
            cursorScreenX = boardStartX + (cursorCol * CELL_WIDTH);
            
            attron(A_REVERSE);
            for (i = 0; i < CELL_HEIGHT; i++) {
                mvprintw(cursorScreenY + i, cursorScreenX, "     ");
            }
            attroff(A_REVERSE);
            
            /* we need to redraw the cell content to preserve the mark */
            drawCell(cursorRow, cursorCol, board[cursorRow][cursorCol], boardStartY, boardStartX);
            
            refresh();
            
            ch = getch(); /* hold the our input*/
            
            switch (ch) {
                case KEY_UP:
                    if (cursorRow > 0) cursorRow--;
                    break;
                case KEY_DOWN:
                    if (cursorRow < BOARD_SIZE - 1) cursorRow++;
                    break;
                case KEY_LEFT:
                    if (cursorCol > 0) cursorCol--;
                    break;
                case KEY_RIGHT:
                    if (cursorCol < BOARD_SIZE - 1) cursorCol++;
                    break;
                case 10:  /* enter key */
                case ' ': 
                    
                    if (board[cursorRow][cursorCol] == 0) { 
                        board[cursorRow][cursorCol] = currentPlayer; /* place mark if that spot is empty */
                        
                        winner = checkWinner(board);  /* check if there's a winner */
                        if (winner != 0) {
                            gameOver = 1;
                            break;
                        }
                        
                        if (isBoardFull(board)) { /* check for a tie */
                            gameOver = 1;
                            winner = -1;
                            break;
                        }
                        
                        /* flip players */
                        currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    }
                    break;
                case 'q':
                case 'Q':
                    endwin();
                    return 0;
            }
        }
        
        /* the game is over so display the result*/
        clear();
        box(stdscr, 0, 0);
        getmaxyx(stdscr, row, col);
        
        if (winner == 1) {
            showWinnerMessage("Player X wins!", row, col);
        } else if (winner == 2) {
            showWinnerMessage("Player O wins!", row, col);
        } else {
            showWinnerMessage("It's a tie!", row, col);
        }

        char playAgainMsg[] = "Play Again? (y/n)";
        mvprintw(row / 2 + 2, (col - strlen(playAgainMsg)) / 2, "%s", playAgainMsg);
        refresh();

        int choice;
        while(1) {
            choice = getch();
            if (choice == 'y' || choice == 'Y'){
                playAgain = 1;
                break;
            } else if (choice == 'n' || choice == 'N') {
                playAgain = 0;
                break;
            }
        }
    }

    return 0;
}

static void drawBoard(int board[BOARD_SIZE][BOARD_SIZE], int startY, int startX) {
    int i, j;
    int lineX, lineY;
    
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            drawCell(i, j, board[i][j], startY, startX);
        }
    }
    
    /* draw the vertical lines */
    for (i = 1; i < BOARD_SIZE; i++) {
        lineX = startX + (i * CELL_WIDTH);
        for (j = 0; j < BOARD_SIZE * CELL_HEIGHT; j++) {
            mvaddch(startY + j, lineX, '|');
        }
    }
    
    /* draw the horizontal horizontal */
    for (i = 1; i < BOARD_SIZE; i++) {
        lineY = startY + (i * CELL_HEIGHT);
        for (j = 0; j < BOARD_SIZE * CELL_WIDTH; j++) {
            mvaddch(lineY, startX + j, '-');
        }
    }
}

static void drawCell(int row, int col, int value, int startY, int startX) {
    int cellY = startY + (row * CELL_HEIGHT);
    int cellX = startX + (col * CELL_WIDTH);
    int centerY = cellY + (CELL_HEIGHT / 2);
    int centerX = cellX + (CELL_WIDTH / 2);
    
    if (value == 1) {  /* X */
        mvaddch(centerY - 1, centerX - 1, '\\');
        mvaddch(centerY - 1, centerX + 1, '/');
        mvaddch(centerY, centerX, 'X');
        mvaddch(centerY + 1, centerX - 1, '/');
        mvaddch(centerY + 1, centerX + 1, '\\');
    } else if (value == 2) {  /* O */
        mvaddch(centerY - 1, centerX, '-');
        mvaddch(centerY, centerX - 1, '(');
        mvaddch(centerY, centerX, 'o');
        mvaddch(centerY, centerX + 1, ')');
        mvaddch(centerY + 1, centerX, '-');
    }
}

/*
 * returns 0 if there are no winners
*/
static int checkWinner(int board[BOARD_SIZE][BOARD_SIZE]) {
    int i;
    
    for (i = 0; i < BOARD_SIZE; i++) {   /* check rows */
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return board[i][0];
        }
    }
    
    for (i = 0; i < BOARD_SIZE; i++) { /* check columns */
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }
    
    /* check diagonals */
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0];
    }
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2];
    }
    
    return 0;
}

static int isBoardFull(int board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

static void showWinnerMessage(const char *message, int row, int col) {
    attron(A_BOLD);
    mvprintw(row / 2, (col - strlen(message)) / 2, "%s", message);
    attroff(A_BOLD);
}
