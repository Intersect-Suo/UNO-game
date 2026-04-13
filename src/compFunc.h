#ifndef COMPFUNC
#define COMPFUNC
#include <stdio.h>
#include "baseFunc.h"

// Before gaming

// Initialize all parameters
void initialize(Deck *stockPile_p, DiscardPile *discardPile_p, int *first_player_index, Card *initialCard_p, int *breakloop_p, int *current_r_p, int n, int c, int d, int demo_mode, FILE *fid, Node **head_pp, Node **headStable, int *isClockWise);

// Gaming

// Draw several cards
int draw_several_cards(FILE *fid, Node *head, DiscardPile *discardPile_p, Deck *stockPile_p, int demo_mode);

// Play one card
void play_one_card(Node **head, DiscardPile *discardPile_p, Deck *stockPile_p, int demo_mode, FILE *fid, int *isClockWise, int *breakLoop_p, int *current_r_p);

// Play one round
void play_one_round(Deck *stockPile_p, DiscardPile *discardPile_p, int *breakLoop_p, int *current_r_p, int n, int r, FILE *fid, int demo_mode, Node **head, Node *headStable, int *isClockWise);

#endif
