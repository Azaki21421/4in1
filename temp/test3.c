#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 7
#define HEIGHT 6
#define EMPTY ' '
#define PLAYER1 'X'
#define PLAYER2 'O'
#define WIN_COUNT 4

typedef struct {
    char board[HEIGHT][WIDTH];
    char player1[50];
    char player2[50];
    int moves;
} Game;

void initializeBoard(Game *game) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            game->board[i][j] = EMPTY;
        }
    }
    game->moves = WIDTH * HEIGHT;
}

void printBoard(Game *game) {
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("| %c ", game->board[i][j]);
        }
        printf("|\n");
    }
    printf("  1   2   3   4   5   6   7\n");
}

int isFullColumn(Game *game, int col) {
    return game->board[0][col] != EMPTY;
}

int placeSymbol(Game *game, int col, char symbol) {
    if (col < 0 || col >= WIDTH || isFullColumn(game, col)) {
        printf("Invalid move. Choose a different column.\n");
        return 0;
    }
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (game->board[i][col] == EMPTY) {
            game->board[i][col] = symbol;
            game->moves--;
            return 1;
        }
    }
    return 0;
}

int checkWin(Game *game, char symbol) {
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            if (game->board[row][col] == symbol) {
                if (col <= WIDTH - WIN_COUNT &&
                    game->board[row][col+1] == symbol &&
                    game->board[row][col+2] == symbol &&
                    game->board[row][col+3] == symbol) return 1;

                if (row <= HEIGHT - WIN_COUNT) {
                    if (game->board[row+1][col] == symbol &&
                        game->board[row+2][col] == symbol &&
                        game->board[row+3][col] == symbol) return 1;
                    if (col <= WIDTH - WIN_COUNT &&
                        game->board[row+1][col+1] == symbol &&
                        game->board[row+2][col+2] == symbol &&
                        game->board[row+3][col+3] == symbol) return 1;
                    if (col >= WIN_COUNT - 1 &&
                        game->board[row+1][col-1] == symbol &&
                        game->board[row+2][col-2] == symbol &&
                        game->board[row+3][col-3] == symbol) return 1;
                }
            }
        }
    }
    return 0;
}

void saveGame(Game *game, int id) {
    FILE *file = fopen("result.txt", "a");
    if (file == NULL) {
        printf("Error saving game.\n");
        return;
    }
    fprintf(file, "%d,%s,%s,%d\n", id, game->player1, game->player2, game->moves);
    fclose(file);
    printf("Game saved with ID %d.\n", id);
}

int loadGame(Game *game, int id) {
    FILE *file = fopen("result.txt", "r");
    if (file == NULL) {
        printf("Error loading game.\n");
        return 0;
    }
    int foundId;
    while (fscanf(file, "%d,%[^,],%[^,],%d\n", &foundId, game->player1, game->player2, &game->moves) == 4) {
        if (foundId == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    printf("Game with ID %d not found.\n", id);
    return 0;
}

void startGame(Game *game) {
    char currentSymbol = PLAYER1;
    while (1) {
        printBoard(game);
        printf("Player %s (%c) move: Enter column (1-7, or 0 to save): ",
               currentSymbol == PLAYER1 ? game->player1 : game->player2, currentSymbol);
        int col;
        scanf("%d", &col);

        if (col == 0) {
            static int saveId = 1;
            saveGame(game, saveId++);
            continue;
        }
        if (placeSymbol(game, col - 1, currentSymbol)) {
            if (checkWin(game, currentSymbol)) {
                printBoard(game);
                printf("Congratulations, player %s (%c) wins!\n",
                       currentSymbol == PLAYER1 ? game->player1 : game->player2, currentSymbol);
                break;
            }
            if (game->moves == 0) {
                printf("Draw! The board is full.\n");
                break;
            }
            currentSymbol = (currentSymbol == PLAYER1) ? PLAYER2 : PLAYER1;
        }
    }
}

void mainMenu() {
    Game game;
    while (1) {
        printf("Main menu:\n1.) New Game\n2.) Load Game\n3.) Exit\nChoose an option: ");
        int choice;
        scanf(" %9s", &choice);

        if (choice == 1) {
            printf("Enter player X name: ");
            scanf("%s", game.player1);
            printf("Enter player O name: ");
            scanf("%s", game.player2);
            initializeBoard(&game);
            startGame(&game);
        } else if (choice == 2) {
            int id;
            printf("Enter ID to load: ");
            scanf("%d", &id);
            if (loadGame(&game, id)) {
                startGame(&game);
            }
        } else if (choice == 3) {
            printf("Exiting the game.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
