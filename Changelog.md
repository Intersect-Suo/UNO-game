
# Change Log  
  
All notable changes to this project will be documented in this file.  
  
## [3.0.0] - 2024-11-19
  
### Added  
  
- Use double circular linked list to implement the "reverse" effect of "Q"
  
### Changed  
  
- Change the usage of "scanf()" in the "play_one_card" function in "compFunc.c" so that the program won't crash if players enter an invalid input.  
- Type numbers (e.g. 2) instead of characters (e.g. Spades Jack) to play a card.
  
### Fixed  
  
- Address the issue of "memory leak" by freeing the memory allocated for the cardArrays of players
- Address the issue of "segmentation fault" when the discardPile and the stockPile are both exhausted by stopping drawing cards using the "able_to_draw" function
