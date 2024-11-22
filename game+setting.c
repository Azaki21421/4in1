// library import
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// distribute values
#define DEFAULT_WIDTH 7
#define DEFAULT_HEIGHT 6
#define DEFAULT_WIN_COUNT 4

char EMPTY = ' ';
char PLAYER1 = 'X';
char PLAYER2 = 'O';

int WIDTH = DEFAULT_WIDTH;
int HEIGHT = DEFAULT_HEIGHT;
int WIN_COUNT = DEFAULT_WIN_COUNT;

char board[100][100];
char player1[50];
char player2[50];
int moves;

void initializeBoard() { // check board
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = EMPTY;
        }
    }
    moves = WIDTH * HEIGHT;
}

void printBoard() { // draw board on screen
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("| %c ", board[i][j]);  // column wall
        }
        printf("|\n");
    }
    for (int j = 1; j <= WIDTH; j++) {  // column number
        printf("  %d ", j);
    }
    printf("\n");
}

int isFullColumn(int col) {
    return board[0][col] != EMPTY;
}

int placeSymbol(int col, char symbol) {  // check for possibility to place symbol
    if (col < 0 || col >= WIDTH || isFullColumn(col)) {
        printf("Invalid move. Choose a different column.\n");
        return 0;
    }
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (board[i][col] == EMPTY) {
            board[i][col] = symbol;
            moves--;
            return 1;
        }
    }
    return 0;
}

int checkWin(char symbol, int *winRow, int *winCol, int *winDir) {
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            if (board[row][col] == symbol) {
                // horizontal check for win
                if (col <= WIDTH - WIN_COUNT &&
                    board[row][col + 1] == symbol &&
                    board[row][col + 2] == symbol &&
                    board[row][col + 3] == symbol) {
                    *winRow = row;
                    *winCol = col;
                    *winDir = 0;
                    return 1;
                }

                // vertical check
                if (row <= HEIGHT - WIN_COUNT) {
                    if (board[row + 1][col] == symbol &&
                        board[row + 2][col] == symbol &&
                        board[row + 3][col] == symbol) {
                        *winRow = row;
                        *winCol = col;
                        *winDir = 1;
                        return 1;
                    }

                    // diag right-down
                    if (col <= WIDTH - WIN_COUNT &&
                        board[row + 1][col + 1] == symbol &&
                        board[row + 2][col + 2] == symbol &&
                        board[row + 3][col + 3] == symbol) {
                        *winRow = row;
                        *winCol = col;
                        *winDir = 2;
                        return 1;
                    }

                    // diag left-down
                    if (col >= WIN_COUNT - 1 &&
                        board[row + 1][col - 1] == symbol &&
                        board[row + 2][col - 2] == symbol &&
                        board[row + 3][col - 3] == symbol) {
                        *winRow = row;
                        *winCol = col;
                        *winDir = 3;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void highlightWinningLine(int winRow, int winCol, int winDir) {  // change symbol for winning line
    for (int i = 0; i < WIN_COUNT; i++) {
        switch (winDir) {
            case 0: // horizontal
                board[winRow][winCol + i] = 'Y';
            break;
            case 1: // vertical
                board[winRow + i][winCol] = 'Y';
            break;
            case 2: // diag right-down
                board[winRow + i][winCol + i] = 'Y';
            break;
            case 3: // diag left-down
                board[winRow + i][winCol - i] = 'Y';
            break;
        }
    }
}

int getNextGameId() {  // receiving id from file
    FILE *file = fopen("result.txt", "r");
    if (!file) {
        return 1;
    }
    int id, maxId = 0;
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,", &id);
        if (id > maxId) {
            maxId = id;
        }
    }
    fclose(file);
    return maxId + 1;
}

void saveGame(int id) {  // save game to file
    FILE *file = fopen("result.txt", "a");  // open
    if (!file) { // check exist
        printf("Error saving game.\n");
        return;
    }
    fprintf(file, "%d,%s,%s,%d,%d,%d,%d\n", id, player1, player2, WIDTH, HEIGHT, WIN_COUNT, moves);  //print from file
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fputc(board[i][j], file);
        }
        fputc('\n', file);
    }
    fclose(file);  //close file
    printf("Game saved with ID %d.\n", id);
}

void listAllSavedGames() {
    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }
    char line[200];
    printf("Saved games:\n");
    while (fgets(line, sizeof(line), file)) {
        int id, freeFields, width, height, winCount;
        char p1[50], p2[50];
        if (sscanf(line, "%d,%[^,],%[^,],%d,%d,%d,%d", &id, p1, p2, &width, &height, &winCount, &freeFields) == 7) {
            printf("ID: %d, '%s', '%s', Free fields: %d\n", id, p1, p2, freeFields);
        }
    }
    fclose(file);
}

void listSavedGamesForPlayer() {
    char playerName[50];
    printf("Enter player name: ");
    scanf("%s", playerName);

    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        int id, freeFields, width, height, winCount;
        char p1[50], p2[50];
        if (sscanf(line, "%d,%[^,],%[^,],%d,%d,%d,%d", &id, p1, p2, &width, &height, &winCount, &freeFields) == 7) {
            if (strcmp(playerName, p1) == 0 || strcmp(playerName, p2) == 0) {
                printf("ID: %d, '%s', '%s', Free fields: %d\n", id, p1, p2, freeFields);
                found = 1;
            }
        }
    }
    fclose(file);

    if (!found) {
        printf("No games found for player '%s'.\n", playerName);
    }
}

void showGameBoard() {  // show board from save
    int id;
    printf("Enter game ID: ");
    scanf("%d", &id);

    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        int foundId, width, height, winCount, moves;
        char p1[50], p2[50];
        if (sscanf(line, "%d,%[^,],%[^,],%d,%d,%d,%d", &foundId, p1, p2, &width, &height, &winCount, &moves) == 7) {
            if (foundId == id) {
                printf("Player 1: %s, Player 2: %s\n", p1, p2);
                for (int i = 0; i < height; i++) {
                    fgets(line, sizeof(line), file);
                    for (int j = 0; j < width; j++) {
                        printf("| %c ", line[j]);
                    }
                    printf("|\n");
                }
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
    printf("Game with ID %d not found.\n", id);
}

void loadSavedGame() {  // load saved game
    int id;
    printf("Enter game ID to load: ");
    while (scanf("%d", &id)) {
        if (loadGame(id)) {
            startGame();
            return;
        }
        printf("Invalid ID. Please enter a valid game ID: ");
    }
}

void loadMenu() {  // menu on choice 2
    while (1) {
        printf("\nLoad Menu:\n");
        printf("1. List all saved games\n");
        printf("2. List all saved games for a particular player\n");
        printf("3. Show the board of one of the saved games\n");
        printf("4. Load a game\n");
        printf("5. Return to main menu\n");
        printf("Choose an option: ");
        int option;
        scanf("%d", &option);

        switch (option) {
            case 1:
                listAllSavedGames();
                break;
            case 2:
                listSavedGamesForPlayer();
                break;
            case 3:
                showGameBoard();
                break;
            case 4:
                loadSavedGame();
                return;
            case 5:
                return;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
}

int loadGame(int id) {  // load game
    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("Error loading game.\n");
        return 0;
    }
    int foundId, w, h, wc;
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%[^,],%[^,],%d,%d,%d,%d", &foundId, player1, player2, &w, &h, &wc, &moves) == 7) {
            if (foundId == id) {
                WIDTH = w;
                HEIGHT = h;
                WIN_COUNT = wc;
                for (int i = 0; i < HEIGHT; i++) {
                    fgets(line, sizeof(line), file);
                    for (int j = 0; j < WIDTH; j++) {
                        board[i][j] = line[j];
                    }
                }
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    printf("Game with ID %d not found.\n", id);
    return 0;
}

void startGame() {  // game on choice 1
    char currentSymbol = PLAYER1;
    int winRow, winCol, winDir;
    while (1) {
        printBoard();  // draw board
        printf("Player %s (%c), enter column (1-%d, 0 to save, or -1 to return to menu): ",
               currentSymbol == PLAYER1 ? player1 : player2, currentSymbol, WIDTH);

        char input[10];
        int col;

        while (1) {
            scanf("%s", input);  // receive user inp
            if (sscanf(input, "%d", &col) == 1) {  // check
                break;
            }
            printf("Invalid input. Please enter a number between -1 and %d: ", WIDTH);
        }

        if (col == 0) {  // save
            int saveId = getNextGameId();
            saveGame(saveId);
            continue;
        } else if (col == -1) {  // exit from game
            return;
        } else if (col < 1 || col > WIDTH) {  // check
            printf("Invalid column. Choose a number between 1 and %d.\n", WIDTH);
            continue;
        }

        if (placeSymbol(col - 1, currentSymbol)) {  // check win
            if (checkWin(currentSymbol, &winRow, &winCol, &winDir)) {
                highlightWinningLine(winRow, winCol, winDir);
                printBoard();
                printf("Congratulations, %s (%c) wins!\n",
                       currentSymbol == PLAYER1 ? player1 : player2, currentSymbol);

                printf("Would you like to start a new game? (y/n): ");
                char choice;
                scanf(" %c", &choice);
                if (choice == 'y' || choice == 'Y') {  // again or return
                    initializeBoard();
                    return startGame();
                } else {
                    return;
                }
            }
            if (moves == 0) {  // no one win
                printf("Draw! The board is full.\n");
                break;
            }
            currentSymbol = (currentSymbol == PLAYER1) ? PLAYER2 : PLAYER1;
        }
    }
}

void settingsMenu() {  // customization
    printf("Settings:\n");
    printf("1. Change board width\n");
    printf("2. Change board height\n");
    printf("3. Change player symbols\n");
    printf("4. Change win condition\n");
    printf("5. Exit settings\n");
    printf("Choose an option: ");
    int option;
    scanf("%d", &option);

    if (option == 1) {
        printf("Enter new width (minimum 4): ");
        scanf("%d", &WIDTH);
        if (WIDTH < 4) WIDTH = DEFAULT_WIDTH;
    } else if (option == 2) {
        printf("Enter new height (minimum 4): ");
        scanf("%d", &HEIGHT);
        if (HEIGHT < 4) HEIGHT = DEFAULT_HEIGHT;
    } else if (option == 3) {
        printf("Enter new symbol for Player 1: ");
        scanf(" %c", &PLAYER1);
        printf("Enter new symbol for Player 2: ");
        scanf(" %c", &PLAYER2);
    } else if (option == 4) {
        printf("Enter new win condition (minimum 4): ");
        scanf("%d", &WIN_COUNT);
        if (WIN_COUNT < 4) WIN_COUNT = DEFAULT_WIN_COUNT;
    } else if (option == 5) {
        return;
    }
    initializeBoard();
}

void mainMenu() {  // first menu
    while (1) {
        printf("Main menu:\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        printf("3. Settings\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        char choice[10];
        scanf("%s", choice);

        if (strlen(choice) != 1 || !isdigit(choice[0])) {  // input check
            printf("Invalid input. Please enter 1, 2, 3, or 4.\n");
            continue;
        }

        if (choice[0] == '1') {
            printf("Enter Player 1 name: ");
            scanf("%s", player1);
            printf("Enter Player 2 name: ");
            scanf("%s", player2);
            initializeBoard();
            startGame();
        } else if (choice[0] == '2') {
            loadMenu();
        } else if (choice[0] == '3') {
            settingsMenu();
        } else if (choice[0] == '4') {
            break;
        }
    }
}

int main() {  // main
    mainMenu();
    return 0;
}
