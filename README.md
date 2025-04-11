# Tic-Tac-Toe Game

This is a simple tic tac toe game created using raylib.

## Features

* **Graphical User Interface:** Built with the raylib library
* **Gameplay Modes:**
    * **Two-Player:** Local multiplayer mode.
    * **Single-Player vs. AI:** Play against the computer with multiple difficulty levels:
        * Easiest (Naive Bayes AI)
        * Easy (Neural Network AI)
        * Medium (Minimax algorithm with limited search depth)
        * Hard (Minimax algorithm with full search depth)
* **Input:** Full mouse support for making moves and interacting with menus/buttons.
* **Core Game Logic:**
    * Implements standard Tic-Tac-Toe rules.
    * Robust win and draw condition checking (includes an optimized assembly version for draw checks).
    * Manages alternating turns between players.
    * Keeps score across consecutive games (resets on return to menu).
    * Option to "Continue Playing" (start a new round with same settings/scores) or "End Game" (return to main menu) after a match concludes.
* **State Management:** Handles various game states like Main Menu, Difficulty Selection, Instructions, Active Gameplay, Game Over (Win/Draw), and Exit Confirmation.
* **Visuals & Audio:**
    * Clear visual representation of the game board, X's, and O's.
    * Highlights the winning line upon victory.
    * Background music with an on/off toggle button available on most screens.
    * Sound effects for UI interactions (button clicks), placing marks, winning, and draws.
    * Dedicated screens for the Main Menu, How To Play instructions, Difficulty Selection, and Exit Confirmation.
    * Smooth transitions with messages for game start and game over sequences.
    * Responsive UI with button hover effects.
    * Adapts grid layout on window resize.
* **Technical Implementation:**
    * Uses efficient bitboards to represent the game state.
    * Loads pre-trained AI models (Neural Network, Naive Bayes) from external files.
    * Employs memoization to optimize certain UI calculations.


## Building the Game

Please see BUILD.md

## Acknowledgements

- The tic-tac-toe game board image is sourced from [Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Tic-tac-toe-game-1.svg).
- Icon by Pixel perfect, available on [Freepik](https://www.freepik.com/icon/tic-tac-toe_2911080).
- The pixel perfect flat icon for tic-tac-toe is provided by Freepik.
- Some sound effects were sourced from [Fesliyan Studios](https://www.fesliyanstudios.com).
- Background music "8 Bit Retro Funk" by David Renda, available for royalty-free download.
- Music off/on icons by FontAwesome Free 6.6