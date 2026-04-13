#ifndef BASEFUNC
#define BASEFUNC
#include <stdio.h>
#include "printing.h"

// Before gaming

// Initialize a node
void initNode(Node **head, int c, FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode);

// Add a node
void addNode(Node **head, int c, FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode);

// Renew the player data in the node after a round
void renewList(Node *head, int n, int c, FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode);

// Decide which player plays first
int firstPlayer_initCard(int playerNum, Deck *stockPile_p, DiscardPile *discardPile_p, Card *initialCard_p, Node **head);

// Shuffle a pile
void shuffle(Deck *stockPile_p);

// Gaming

// Judge if there is no card left in both SP and DP
int able_to_draw(FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode);

// Draw a card
void draw_one_card(FILE *fid, Node *head, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode);

// Judge if the player has card ranked n (eg: 2/3/7/J/Q)
int is_rank_contained(Node *head, int rank);

// Judge if the player has played a card ranked n (eg: 2/3/7/J/Q)
int is_rank_played(Node *head, int remove_position, int n);

// Judge if the player has cards that have Suit s and Rank n
int is_suit_rank_contained(Node *head, int suit, int rank);

// Judge if the player plays a card that has Suit s and Rank n
int is_suit_rank_played(Node *head, int remove_position, int suit, int rank);

// Judge if the card played is valid
int isValidCard(DiscardPile *discardPile_p, Node *head, int remove_position);

// Judge if all the card a player has is invalid
int is_all_invalid(DiscardPile *discardPile_p, Node *head);

// Record the card played
void record_pre_card(DiscardPile *discardPile_p, Node *head, int remove_position, int *isClockWise);

// Remove one card
void remove_one_card(Node *head, int remove_position);

// Determine "clockwise" or "counterclockwise" and jump to the next player
void moveNext(Node **head, const int *isClockWise);

// Increase the space of the discard pile (without setting values)
void increase_discard_pile_space(DiscardPile *discardPile_p);

// Renew the discard pile. The only card in the new DP is the last card of the old DP
void renew_discard_pile(DiscardPile *discardPile_p);

// Reduce the stock pile
void reduce_stock_pile(Deck *stockPile_p);

// Restore the stock pile if it has no cards left
void restore_stock_pile(FILE *fid, DiscardPile *discardPile_p, Deck *stockPile_p, int demo_mode);

// After gaming

// End the round if a player has no cards in hand
int endRound(Node **head, int *current_r_p, int demo_mode, FILE *fid);

// Calculate the penalty of each round and add them up to the player's score
void calculatePenalty(Node *head, int n);

// Determine who wins eventually (Maybe more than one winner)
void winner(FILE *fid, Node *head, int n, int demo_mode);

// Free a list of nodes
void freeList(Node **head, int n);

// Free allocated memory
void freeMemory(Deck *stockPile_p, DiscardPile *discardPile_p, int n, Node **head);

#endif
