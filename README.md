## Introduction

This project implements a beloved game **_One Card_** using C language.

### Game overview

 One Card is a shedding card game for n players over r rounds. Players start with c cards each from d decks, and an extra card determines the play order. The game is played counter-clockwise; players must follow the suit or rank of the last played card. If unable to play, they draw from the stock pile. The game ends when a player runs out of cards, with others penalized for remaining cards. The player with the highest score after r rounds wins.  

### Card description  

**_Function Card:_**  

- **Rank 2 (Attack):** Forces the next player to draw two cards from the stock pile.
- **Rank 3 (Attack):** Forces the next player to draw three cards from the stock pile.
- **Rank 7 (Defense):** Cancels an attack, meaning no cards are drawn if a 2 or 3 was played previously.
- **Queen (Action):** Reverses the playing order from clockwise to counter-clockwise or vice versa and redirect an attack if there is any.
- **Jack (Action):** Skips the next player's turn and skips an attack if there is any.

**_Regular Cards:_**  

- No special effect; used to match the rank or suit of the previously played card.

**_Note:_**  

- Players can only play a function card when attacked, and the "attack" effect is cumulative.
- If a player is attacked and plays a "Q", then the next player can play no cards but only draw cards.  

## Features and Changes

- Players only need to type numbers (e.g. 2) instead of lots of characters (e.g. Spades Jack) to play a card.
- The program won't crash if the user enter an invalid input by checking the return value of _scanf()_.
- The player stops drawing cards if the discard pile and the stock pile are both exhausted when drawing.

## Explanation

**_Program structure_**  

The project are organized into five parts.  

- **structure.h (Layer 1):** Defines some structures to represent objects like decks and players.
- **printing.c (Layer 1):** Contains some functions to print the game process in both the command line and the log file.
- **baseFunc.c (Layer 2):** Contains some basic functions which focuses on specific tasks, like drawing cards and freeing memory, and calls functions from Layer 1.
- **compFunc.c (Layer 3):** Contains some complex functions which calls functions from Layer 1 and Layer 2 to initialize parameters and run the game.
- **main.c (Layer 4):** Parses the command line argument, create some parameters and calls functions from Layer 3.

**_Determine the playing order_**  

- Before the game begins, each player is given another card to compare. The player with the smallest card rank becomes the first player. If there are more than one player with the smallest rank, then we start with the player who have the smallest rank and the smallest player ID (e.g. Player 1 < Player 2 < Player 3).

**_Default: Counter-clockwise playing_**  

- The game is played counter-clockwise. For example, if there are 4 players and the first player is player 3, then the initial sequence is **3 -> 2 -> 1 -> 4 -> 3**.  

**_Printing whether the function of function cards is used_**  

- If the function of some function cards (2/3/Jack) has not been used, the player mode will display "(Function card)" after printing the card; otherwise, the function has already been used.  

## Usage example

The project interacts with users through the command line. Users can choose whether to play the game themselves (player-mode) or to let the computer play the game (demo-mode).  

**_Some parameters:_**

- -h|--help print this help message
- --log filename write the logs in filename (default: onecard.log)
- -n n|--player-number= n n players, n must be larger than 2 (default: 4)
- -c c|--initial-cards= c deal c cards per player, c must be at least 2 (default: 5)
- -d d|--decks= d use d decks 52 cards each, d must be at least 2 (default: 2)
- -r r|--rounds= r play r rounds, r must be at least 1 (default: 1)
- -a|--auto run in demo mode

**Note that there is a space in the long command !!! (e.g. "--player-number= n")**  

**_Player mode:_**  

- Just type "**./onecard --log test -n 3 -c 2 -d 2 -r 2**". Note that if you don't designate a value for a parameter, its value will be replaced by the default one. (e.g. "**./onecard --log test --initial-cards=2 -d 3**" means starting the game with the log file name "test", 4 players, 2 cards per player, 3 decks per player, and 1 round)

**_Demo mode:_**  

- Just type "**./onecard --log test -n 3 -c 2 -d 2 -r 2 -a**" or "**./onecard --log test -n 3 -c 2 -d 2 -r 2 --auto**".

**_Input format:_**

- **To play a card, simply enter the serial number of the card.**  

- e.g.  
- When command line displays:  

``` markdown
Previous card: Spades 4  
Player 2 Cards: 1: Spades Queen   2: Hearts Queen  
Please enter the serial number of the card you want to play:  
```

- If you want to play the card "Spades Queen", just enter 1.  

## Bugs section

- None

## Note

- **_A space in the long command (e.g. "--player-number= n")_**
- **_To compile a program, you can use the following command: gcc main.c baseFunc.c compFunc.c printing.c -Werror -Wall -Wextra -Wno-unused-result -Wconversion -Wvla -Wpedantic -std=c11 -o onecard_**
