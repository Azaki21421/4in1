# Connect Four Game in C

This repository contains an implementation of the classic **Connect Four** game in C. It allows players to customize the game settings, save/load game states, and replay previous games. The code supports features such as dynamic board sizes and win conditions, ensuring a versatile gaming experience.

## Features

- **Customizable Game Settings**:
  - Change board width and height.
  - Modify player symbols.
  - Set the win condition (number of consecutive symbols required to win).

- **Save and Load Games**:
  - Save the current game state to a file.
  - Load previously saved games by game ID.
  - List all saved games or filter by player name.

- **Game Mechanics**:
  - Automatic detection of winning conditions (horizontal, vertical, diagonal).
  - Highlight the winning sequence on the board.
  - Prevent invalid moves (e.g., selecting full columns or out-of-bound moves).

## Requirements

- A C compiler (e.g., GCC).
- Basic understanding of terminal operations.
- Windows system (.exe release)

## Example Gameplay

```
Main menu:
1. New Game
2. Load Game
3. Settings
4. Exit
Choose an option: 1
Enter Player 1 name: Alice
Enter Player 2 name: Bob

|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
  1   2   3   4   5   6   7

Player Alice (X), enter column (1-7, 0 to save, or -1 to return to menu): 4
```
