#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "printing.h"

void pauseCmd(void) // Pause before clearing the screen
{   printf("Press Enter to continue...\n");while (getchar() != '\n');}

void clear_screen(int demo_mode) // Clear the screen
{   if (demo_mode == 0){pauseCmd();system(CLEAR_CMD);}}

void print_one_card(Card card)  // Print one card
{   int suit = card.Suit; int rank = card.Rank;

    switch (suit){
    case SPADES: printf("Spades ");break;
    case HEARTS: printf("Hearts ");break;
    case DIAMONDS: printf("Diamonds ");break;
    case CLUBS: printf("Clubs ");break;
    default: break;}

    switch (rank){
    case TWO: printf("2");break;
    case THREE: printf("3");break;
    case FOUR: printf("4");break;
    case FIVE: printf("5");break;
    case SIX: printf("6");break;
    case SEVEN: printf("7");break;
    case EIGHT: printf("8");break;
    case NINE: printf("9");break;
    case TEN: printf("10");break;
    case JACK: printf("Jack");break;
    case QUEEN: printf("Queen");break;
    case KING: printf("King");break;
    case ACE: printf("ACE");break;
    default: break;}}

void print_one_card_log(FILE *fid, Card card) // Print one card in the log file
{   int suit = card.Suit; int rank = card.Rank;

    switch (suit){
    case SPADES: printLog(fid, "Spades ");break;
    case HEARTS: printLog(fid, "Hearts ");break;
    case DIAMONDS: printLog(fid, "Diamonds ");break;
    case CLUBS: printLog(fid, "Clubs ");break;
    default: break;}

    switch (rank){
    case TWO: printLog(fid, "2");break;
    case THREE: printLog(fid, "3");break;
    case FOUR: printLog(fid, "4");break;
    case FIVE: printLog(fid, "5");break;
    case SIX: printLog(fid, "6");break;
    case SEVEN: printLog(fid, "7");break;
    case EIGHT: printLog(fid, "8");break;
    case NINE: printLog(fid, "9");break;
    case TEN: printLog(fid, "10");break;
    case JACK: printLog(fid, "Jack");break;
    case QUEEN: printLog(fid, "Queen");break;
    case KING: printLog(fid, "King");break;
    case ACE: printLog(fid, "ACE");break;
    default: break;}}

void print_played_card(Node *head, int remove_position) // Print played card
{   printf("Player %d plays: ", head->ind + 1);
    print_one_card(head->player.playerDeck.cardArray[remove_position]);
    printf("\n");}

void print_played_card_log(FILE *fid, Node *head, int remove_position, int demo_mode) // Print played card in the log file
{   if (demo_mode == 0)
    {   printLog(fid, "Player %d plays : ", head->ind + 1);
        print_one_card_log(fid, head->player.playerDeck.cardArray[remove_position]);
        printLog(fid, "\n");}}

void print_player_cards(Node *head) // Print the player's cards
{   if (head->player.playerDeck.cardArray_num != 0)
    {   sortCards(&head->player.playerDeck);        // Sort the player's card
        printf("Player %d Cards: ", head->ind + 1); // Print the player's sorted card
        for (int i = 0; i < head->player.playerDeck.cardArray_num; i++)
        {   printf("%d: ", i + 1);
            print_one_card(head->player.playerDeck.cardArray[i]);
            printf("   ");}
        printf("\n");}}

void print_PC_log(FILE *fid, Node *head, int demo_mode) // Print the player's card in the log file
{   if (demo_mode == 0)
    {   sortCards(&head->player.playerDeck);                // Sort the player's card
        printLog(fid, "Player %d cards : ", head->ind + 1); // Print the player's card
        for (int i = 0; i < head->player.playerDeck.cardArray_num; i++)
        {   print_one_card_log(fid, head->player.playerDeck.cardArray[i]);
            if (i != head->player.playerDeck.cardArray_num - 1)
            {   printLog(fid, " , ");}}
        printLog(fid, "\n");}}

void print_pre_card(DiscardPile discardPile, int demo_mode) // Print the previously played card
{   if (demo_mode == 0)
    {   printf("Previous card: ");
        print_one_card(discardPile.discardDeck.cardArray[discardPile.discardDeck.cardArray_num - 1]);
        if ((discardPile.ToPlus[discardPile.discardDeck.cardArray_num - 1] != 0) || (discardPile.ToSkip[discardPile.discardDeck.cardArray_num - 1] != 0)) // If the previous card is a function card and its effect is unused, add (function)
        {   printf("  (Function Card)");}
        printf("\n");}}

void print_deal(Node *head, int n) // Print the card-dealing result
{   Node *current = head;
    printf("Dealing cards ...\n\n");
    for (int i = 0; i < n; i++)
    {   print_player_cards(current);
        current = current->nxt;}}

void print_deal_log(FILE *fid, Node *head, int n, int demo_mode) // Print the card-dealing result in the log file
{   if (demo_mode == 0)
    {   Node *current = head;
        printLog(fid, "Dealing cards ...\n");
        for (int i = 0; i < n; i++)
        {   print_PC_log(fid, current, demo_mode);
            current = current->nxt;}}}

void print_order_initCard(DiscardPile discardPile, int n, int first_player_index, Card initialCard) // Print the order of playing and first card
{   printf("\n\nDetermining the playing order ...\n\n");
    for (int i = 0; i < n; i++)
    {   printf("Player %d : ", i + 1);
        print_one_card(discardPile.discardDeck.cardArray[i]);
        printf("\n");}
    printf("The game will start with player %d\n\n\n", first_player_index + 1);
    printf("---- Game start ----\n");
    printf("First card : ");
    print_one_card(initialCard);
    printf("\n");}

void print_order_initCard_log(FILE *fid, DiscardPile discardPile, int n, int first_player_index, Card initialCard, int demo_mode) // Print the order of playing and first card in the log file
{   if (demo_mode == 0)
    {   printLog(fid, "\n\nDetermining the playing order ...\n");
        for (int i = 0; i < n; i++)
        {   printLog(fid, "Player %d : ", i + 1);
            print_one_card_log(fid, discardPile.discardDeck.cardArray[i]);
            printLog(fid, "\n");}
        printLog(fid, "The game will start with player %d\n\n\n", first_player_index + 1);
        printLog(fid, "---- Game start ----\n");
        printLog(fid, "First card : ");
        print_one_card_log(fid, initialCard);
        printLog(fid, "\n");}}

void print_draw_cards(Node *head, int drawTimes) // Print the cards the player draws
{   if (drawTimes == 0)
    {   printf("No card in the stock and discard pile !\n");
        return;}
    else
    {   printf("Player %d draws: ", head->ind + 1);
        for (int i = head->player.playerDeck.cardArray_num - drawTimes; i < head->player.playerDeck.cardArray_num; i++)
        {   print_one_card(head->player.playerDeck.cardArray[i]);
            printf("  ");}
        printf("\n");}}

void print_draw_cards_log(FILE *fid, Node *head, int drawTimes, int demo_mode) // Print the cards the player draws in the log file
{   if (demo_mode == 0)
    {   if (drawTimes == 0)
        {   printf("No card in the stock and discard pile !\n");
            return;}
        else
        {   printLog(fid, "Player %d draws : ", head->ind + 1);
            for (int i = head->player.playerDeck.cardArray_num - drawTimes; i < head->player.playerDeck.cardArray_num; i++)
            {   print_one_card_log(fid, head->player.playerDeck.cardArray[i]);
                if (i != head->player.playerDeck.cardArray_num - 1)
                {   printLog(fid, " , ");}}
            printLog(fid, "\n");}}}

void print_score(Node *head, int n, int current_r) // Print each player's scores
{   Node *current = head;
    printf("\n\n");
    printf("---- Status ----\n");
    printf("Round %d result:\n", current_r);
    for (int i = 0; i < n; i++)
    {   printf("Player %d: %d, total: %d\n", current->ind + 1, -current->player.playerDeck.cardArray_num, current->player.score);
        current = current->nxt;}
    printf("Round %d ends.\n", current_r);}

void print_score_log(FILE *fid, Node *head, int n, int current_r, int demo_mode) // Print each player's scores in the log file
{   if (demo_mode == 0)
    {   Node *current = head;
        printLog(fid, "\n\n---- Status ----\n");
        printLog(fid, "Round %d result:\n", current_r);
        for (int i = 0; i < n; i++)
        {    printLog(fid, "Player %d: %d, total: %d\n", current->ind + 1, -current->player.playerDeck.cardArray_num, current->player.score);
            current = current->nxt;}
        printLog(fid, "Round %d ends.\n", current_r);}}

void print_SP_log(FILE *fid, Deck stockPile, int demo_mode) // Print the stock pile in the log file
{   if (demo_mode == 0)
    {   printLog(fid, "Shuffling cards ...\n");
        printLog(fid, "Shuffle result :\n");
        for (int i = 0; i < stockPile.cardArray_num; i++)
        {   print_one_card_log(fid, stockPile.cardArray[i]);
            if (i != stockPile.cardArray_num - 1)
            {   printLog(fid, " , ");}}
        printLog(fid, "\n\n\n");}}

void print_stock_pile(Deck stockPile) // Print the stock pile
{   printf("\nShuffling cards ...\n");
    printf("Shuffle result :\n\n");
    for (int i = 0; i < stockPile.cardArray_num; i++)
    {   print_one_card(stockPile.cardArray[i]);
        if (i != stockPile.cardArray_num - 1)
        {   printf(", ");}}
    printf("\n\n\n");}

void printLog(FILE *fid, const char *format, ...) // Write the log file
{
    if (fid == NULL) // Open the log file
    {
        fprintf(stderr, "Failed to open the log file !\n");
        exit(1);
    }
    va_list args; // Use variadic argument lists to write log messages.
    va_start(args, format);
    vfprintf(fid, format, args);
    va_end(args);
}

int compareCards(const void *a, const void *b) // Compare two cards for sorting
{
    Card *cardA = (Card *)a;
    Card *cardB = (Card *)b;
    if (cardA->Suit != cardB->Suit) // Compare suits
    {
        return cardA->Suit - cardB->Suit;
    }
    return cardA->Rank - cardB->Rank; // Compare ranks
}

void sortCards(Deck *deck_p) // Sort the cards
{
    qsort(deck_p->cardArray, (size_t)deck_p->cardArray_num, sizeof(Card), compareCards);
}

void print_help(void) // Print the help message
{
    printf("Usage: program [options]\n");
    printf("  -h|--help                print this help message\n");
    printf("  --log filename           write the logs in filename (default: OneCard.log)\n");
    printf("  -n n|--player-number= n  n players, n must be larger than 2 (default: 4)\n");
    printf("  -c c|--initial-cards= c  deal c cards per player, c must be at least 2 (default: 5)\n");
    printf("  -d d|--decks= d          use d decks 52 cards each, d must be at least 2 (default: 2)\n");
    printf("  -r r|--rounds= r         play r rounds, r must be at least 1 (default: 1)\n");
    printf("  -a|--auto                run in demo mode\n");
}

void print_game_setting(int r, int n, int c, int d) // Print the game setting
{
    printf("\n\n\n########################\n");
    printf("#                      #\n");
    printf("# Welcome to One Card! #\n");
    printf("#                      #\n");
    printf("########################\n\n\n");

    printf("---- Initial setup ----\n");
    printf("  Number of rounds: %d\n", r);
    printf("  Number of decks: %d\n", d);
    printf("  Number of players: %d\n", n);
    printf("  Initial cards per player: %d\n", c);
}

void print_game_setting_log(FILE *fid, int r, int n, int c, int d, int demo_mode) // Print the game setting in the log file
{
    if (demo_mode == 0)
    {
        printLog(fid, "########################\n");
        printLog(fid, "#                                            #\n");
        printLog(fid, "#    Welcome to One Card !    #\n");
        printLog(fid, "#                                            #\n");
        printLog(fid, "########################\n\n\n");

        printLog(fid, "---- Initial setup ----\n");
        printLog(fid, "  Number of rounds: %d\n", r);
        printLog(fid, "  Number of decks: %d\n", d);
        printLog(fid, "  Number of players: %d\n", n);
        printLog(fid, "  Initial cards per player: %d\n\n\n", c);
    }
}
