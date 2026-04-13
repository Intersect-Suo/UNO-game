#ifndef PRINTING
#define PRINTING

#include <stdio.h>
#include "structures.h"

// Examine the type of the operating system, and define the command to clear the screen
#if defined(_WIN32) || defined(_WIN64)
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif

// Pause before clearing the screen
void pauseCmd(void);

// Clear the screen
void clear_screen(int demo_mode);

// Print one card
void print_one_card(Card card);

// Print one card in the log file
void print_one_card_log(FILE *fid, Card card);

// Print played card
void print_played_card(Node *head, int remove_position);

// Print played card in the log file
void print_played_card_log(FILE *fid, Node *head, int remove_position, int demo_mode);

// Print the player's cards
void print_player_cards(Node *head_p);

// Print the player's card in the log file
void print_PC_log(FILE *fid, Node *head_p, int demo_mode);

// Print the previously played card
void print_pre_card(DiscardPile discardPile, int demo_mode);

// Print the card-dealing result
void print_deal(Node *head, int n);

// Print the card-dealing result in the log file
void print_deal_log(FILE *fid, Node *head, int n, int demo_mode);

// Print the order of playing and first card
void print_order_initCard(DiscardPile discardPile, int n, int first_player_index, Card initialCard);

// Print the order of playing and first card in the log file
void print_order_initCard_log(FILE *fid, DiscardPile discardPile, int n, int first_player_index, Card initialCard, int demo_mode);

// Print the cards the player draws
void print_draw_cards(Node *head_p, int drawTimes);

// Print the cards the player draws in the log file
void print_draw_cards_log(FILE *fid, Node *head_p, int drawTimes, int demo_mode);

// Print each player's scores
void print_score(Node *head, int n, int current_r);

// Print each player's scores in the log file
void print_score_log(FILE *fid, Node *head, int n, int current_r, int demo_mode);

// Print the stock pile in the log file
void print_SP_log(FILE *fid, Deck stockPile, int demo_mode);

// Print the stock pile
void print_stock_pile(Deck stockPile);

// Write the log file
void printLog(FILE *fid, const char *format, ...);

// Compare two cards
int compareCards(const void *a, const void *b);

// Sort the cards
void sortCards(Deck *deck_p);

// Print the help message
void print_help(void);

// Print the game setting
void print_game_setting(int r, int n, int c, int d);

// Print the game setting in the log file
void print_game_setting_log(FILE *fid, int r, int n, int c, int d, int demo_mode);

#endif
