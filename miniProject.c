// Tic-Tac-Toe Game Simulation with Enhanced AI
// ENGG1410 - Mini Project Part 1
// Fully commented for clarity

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10 // Maximum grid size

// --- Function Prototypes ---
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size);
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size);
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player);
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char player);
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size);
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char ai, char human);
int tryWinOrBlock(char board[MAX_SIZE][MAX_SIZE], int size, char symbol);
void updateScore(int *scoreX, int *scoreO, char winner);

int main() {
    srand(time(NULL));  // random seed for AI

    char board[MAX_SIZE][MAX_SIZE];
    int size;
    int mode; // 1 = PvP, 2 = PvAI

    int scoreX = 0, scoreO = 0;
    char playAgain = 'y';

    printf("===== TIC TAC TOE GAME =====\n");

    // Select grid size
    do {
        printf("Enter board size (3 to 10): ");
        scanf("%d", &size);
    } while (size < 3 || size > 10);

    // Select game mode
    do {
        printf("Choose mode:\n1. Player vs Player\n2. Player vs AI\nEnter: ");
        scanf("%d", &mode);
    } while (mode != 1 && mode != 2);

    while (playAgain == 'y' || playAgain == 'Y') {
        initializeBoard(board, size);

        char currentPlayer = 'X';
        int gameOver = 0;

        while (!gameOver) {

            printBoard(board, size);
            printf("Player %c's turn.\n", currentPlayer);

            // Handle PvP or PvAI mode
            if (mode == 1 || (mode == 2 && currentPlayer == 'X')) {
                playerMove(board, size, currentPlayer);
            } else {
                aiMove(board, size, 'O', 'X');
            }

            // Check for win
            if (checkWin(board, size, currentPlayer)) {
                printBoard(board, size);
                printf("\nPlayer %c WINS!\n", currentPlayer);
                updateScore(&scoreX, &scoreO, currentPlayer);
                gameOver = 1;
            }
            // Check for draw
            else if (checkDraw(board, size)) {
                printBoard(board, size);
                printf("\nIt's a DRAW!\n");
                gameOver = 1;
            }

            // Switch player
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }

        printf("\nScores -> X: %d | O: %d\n", scoreX, scoreO);
        printf("Play again? (y/n): ");
        scanf(" %c", &playAgain);
    }

    return 0;
}

// Initialize the board with empty spaces
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = ' ';
}

// Display the board to the screen
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        // Print row
        for (int j = 0; j < size; j++) {
            printf(" %c ", board[i][j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");
        // Row separator
        if (i < size - 1) {
            for (int j = 0; j < size; j++) {
                printf("---");
                if (j < size - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Player manually enters row and column 
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int r, c;

    while (1) {
        printf("Enter row and column (1-%d): ", size);
        scanf("%d %d", &r, &c);

        r--; c--; // convert to array index

        if (r >= 0 && r < size && c >= 0 && c < size && board[r][c] == ' ') {
            board[r][c] = player;
            break;
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

//  Check rows, columns, diagonals for a win
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char p) {

    // Check rows
    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = 0; j < size; j++)
            if (board[i][j] == p) count++;

        if (count == size) return 1;
    }

    // Check columns
    for (int j = 0; j < size; j++) {
        int count = 0;
        for (int i = 0; i < size; i++)
            if (board[i][j] == p) count++;

        if (count == size) return 1;
    }

    // Maindiagonal
    int count = 0;
    for (int i = 0; i < size; i++)
        if (board[i][i] == p) count++;
    if (count == size) return 1;

    // Antidiagonal
    count = 0;
    for (int i = 0; i < size; i++)
        if (board[i][size - 1 - i] == p) count++;
    if (count == size) return 1;

    return 0;
}

// Check if board is full 
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

//  AI Logic: win > block > random 
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char ai, char human) {

    // 1. Check if AI can win first
    if (tryWinOrBlock(board, size, ai)) return;

    // 2. Block human from winning if possible
    if (tryWinOrBlock(board, size, human)) return;

    // pick a random empty cell 
    int r, c;
    do {
        r = rand() % size;
        c = rand() % size;
    } while (board[r][c] != ' ');

    board[r][c] = ai;
}

// Try placing a symbol in empty spots to win/block 
int tryWinOrBlock(char board[MAX_SIZE][MAX_SIZE], int size, char symbol) {

    // Temporary board
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {

            if (board[i][j] == ' ') {
                board[i][j] = symbol;

                if (checkWin(board, size, symbol)) { //if a win is possible
                    if (symbol == 'O') return 1; // AI plays here
                    else { //block human by placing an O
                        board[i][j] = 'O';
                        return 1; 
                    }
                }

                board[i][j] = ' '; // undo
            }
        }
    }
    return 0;
}

//  Update score after each round
void updateScore(int *scoreX, int *scoreO, char winner) {
    if (winner == 'X') (*scoreX)++;
    else if (winner == 'O') (*scoreO)++;
}