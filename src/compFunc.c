#include <stdio.h>
#include <stdlib.h>
#include "compFunc.h"

// Before gaming

// Initialize all parameters
void initialize(Deck *stockPile_p, DiscardPile *discardPile_p, int *first_player_index, Card *initialCard_p, int *breakloop_p, int *current_r_p, int n, int c, int d, int demo_mode, FILE *fid, Node **head_pp, Node **headStable, int *isClockWise)
{
    // Renew the current round
    (*current_r_p)++;
    // Reset the sequence to clockwise
    *isClockWise = 0;
    // Print the round
    printf("\n\n");
    printf("Round %d:\n\n", *current_r_p);
    if (demo_mode == 0)
    {
        printLog(fid, "Round %d:\n", *current_r_p);
    }
    // Initialize the stock pile
    stockPile_p->cardArray_num = 52 * d;
    stockPile_p->cardArray = (Card *)malloc(((size_t)stockPile_p->cardArray_num) * sizeof(Card));
    if (stockPile_p->cardArray == NULL)
    {
        // Handle malloc failure
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    for (int k = 0; k < d; k++)
    {
        for (int i = SPADES; i <= CLUBS; i++)
        {
            for (int j = TWO; j <= ACE; j++)
            {
                stockPile_p->cardArray[index].Suit = i;
                stockPile_p->cardArray[index].Rank = j;
                index++; // Actually, "index" can be replaced with "i * 13 + j"
            }
        }
    }
    // Shuffle the stock pile
    shuffle(stockPile_p);
    // Demo mode
    if (demo_mode == 1)
    {
        // Print the shuffled stock pile
        print_stock_pile(*stockPile_p);
    }
    // Player mode
    else
    {
        // Print the shuffled stock pile in the log file
        print_SP_log(fid, *stockPile_p, demo_mode);
    }
    // Initialize the discardPile
    discardPile_p->discardDeck.cardArray = NULL;
    discardPile_p->discardDeck.cardArray_num = 0;
    discardPile_p->ToPlus = NULL;
    discardPile_p->ToSkip = NULL;
    // Initialize the node
    // If it is the first round, then initialize the node
    if (*current_r_p == 1)
    {
        initNode(head_pp, c, fid, stockPile_p, discardPile_p, demo_mode);
        *headStable = *head_pp;

        for (int i = 1; i < n; i++)
        {
            addNode(head_pp, c, fid, stockPile_p, discardPile_p, demo_mode);
        }
    }
    // If it is not the first round, then renew the player data in the node
    else
    {
        // Find the first player
        *head_pp = *headStable; // "headStable" functions as an anchor
        renewList(*head_pp, n, c, fid, stockPile_p, discardPile_p, demo_mode);
    }
    // Initialize the initialCard and the index of the first player
    *first_player_index = firstPlayer_initCard(n, stockPile_p, discardPile_p, initialCard_p, head_pp);
    // Initialize the breakloop
    *breakloop_p = 0;
    // Demo mode
    if (demo_mode == 1)
    {
        // Print the card-dealing result
        print_deal(*head_pp, n);
    }
    // Player mode
    else
    {
        printf("Dealing cards ...\n\n");
        printf("Dealing cards completed !\n");
        // Print the card-dealing result in the log file
        print_deal_log(fid, *head_pp, n, demo_mode);
    }
    // Print the order of playing and the first card
    print_order_initCard(*discardPile_p, n, *first_player_index, *initialCard_p);
    // Print the order of playing and first card in the log file
    print_order_initCard_log(fid, *discardPile_p, n, *first_player_index, *initialCard_p, demo_mode);
}

// Gaming

// Draw several cards
int draw_several_cards(FILE *fid, Node *head, DiscardPile *discardPile_p, Deck *stockPile_p, int demo_mode)
{
    int plusTimes = discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1];
    if (plusTimes == 0) // the "plus" effect has been used or it is not a card ranked "2" or "3"
    {
        return -1;
    }
    // Check if:  1. There is plusTimes and the previous card isn't "Q"  2. The player' s card has cards whose ranks are the same as the previously played card, or whose suits are the same as the previously played card and ranks are 2 / 3 / 7 / Jack / Q
    else if ((discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Rank != QUEEN) && ((is_rank_contained(head, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Rank)) || (is_suit_rank_contained(head, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, TWO)) || (is_suit_rank_contained(head, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, THREE)) || (is_suit_rank_contained(head, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, SEVEN)) || (is_suit_rank_contained(head, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, JACK)) || (is_suit_rank_contained(head, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, QUEEN))))
    {
        return 2;
    }
    else // The "plus" effect has not been used and the previous card is Q, or simply have no function cards, draw cards
    {
        print_pre_card(*discardPile_p, demo_mode); // Print the previously played card
        if (demo_mode == 0)
        {
            print_player_cards(head); // print the player's original cards
            printf("No card can be discarded !\n");
        }                                   // inform players no card can be played
        for (int i = 0; i < plusTimes; i++) // draw "cnt" cards
        {
            if (able_to_draw(fid, stockPile_p, discardPile_p, demo_mode) == 0) // No card left in both SP and DP. Can't draw cards.
            {
                plusTimes = i;
                break;
            }
            draw_one_card(fid, head, stockPile_p, discardPile_p, demo_mode);
        }
        print_draw_cards(head, plusTimes); // Print the card the player draws
        print_draw_cards_log(fid, head, plusTimes, demo_mode);
        discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1] = 0; // If the player draws several cards, then the effect of the "plus" has been used
        print_player_cards(head);                                                // print the player's new cards
        clear_screen(demo_mode);                                                 // Clear the screen
        return 1;
    }
} // Return 1 to indicate that we don't draw one card even if we have no cards to discard later

// Play one card
void play_one_card(Node **head, DiscardPile *discardPile_p, Deck *stockPile_p, int demo_mode, FILE *fid, int *isClockWise, int *breakLoop_p, int *current_r_p)
{   if (discardPile_p->ToSkip[discardPile_p->discardDeck.cardArray_num - 1] == 1) // Jack : skip 1 player if the the "skip "effect has not been used
    {   print_pre_card(*discardPile_p, demo_mode); // Print the previously played card
        print_player_cards(*head);                 // print the player's original cards
        if (demo_mode == 0)
        {   printf("You are skipped !\n");}
        print_player_cards(*head); // print the player's new cards
        clear_screen(demo_mode);
        discardPile_p->ToSkip[discardPile_p->discardDeck.cardArray_num - 1] = 0; // The "skip "effect has been used
        moveNext(head, isClockWise);return;} // skip the player
    int choice = 0;
    int Is_All_Invalid = is_all_invalid(discardPile_p, *head);
    int drawSeveralCards = draw_several_cards(fid, *head, discardPile_p, stockPile_p, demo_mode); // Draw several cards if the previous card's rank is 2 or 3
    // The previous card played is "2/3" and "plus" is unused, or the previous card played is "Q" and "plus" is unused
    if (drawSeveralCards == 1)
    {   moveNext(head, isClockWise);return;} // Move next because we have draw cards in "draw_several_cards" function
    // If the previous card isn't "+2/+3", or if the previous card is "+2/+3" and "plus" is used, then normally play cards
    else if (drawSeveralCards == -1)
    {   if (Is_All_Invalid == -1) // If all the cards are invalid
        {   if (demo_mode == 0)
            {   print_pre_card(*discardPile_p, demo_mode); // Print the previously played card
                print_player_cards(*head);                 // print the player's original cards
                printf("No card can be discarded !\n");}
            if (able_to_draw(fid, stockPile_p, discardPile_p, demo_mode) == 0) // Draw cards
            {   print_draw_cards(*head, 0);
                print_draw_cards_log(fid, *head, 0, demo_mode);}
            else
            {   draw_one_card(fid, *head, stockPile_p, discardPile_p, demo_mode);
                print_draw_cards(*head, 1);
                print_draw_cards_log(fid, *head, 1, demo_mode);}
            print_player_cards(*head); // print the player's new cards
            clear_screen(demo_mode);
            moveNext(head, isClockWise);
            return;} // A person who has no cards to discard shouldn't discard a card after drawing one card
        else // If one or some cards are valid
        {   if (demo_mode == 1) // The demo-mode
            {   shuffle(&((*head)->player.playerDeck)); // Shuffle the cards to ensure a random card is played
                for (int i = 0; i < (*head)->player.playerDeck.cardArray_num; i++)
                { // Traverse each card until a valid card is found
                    if (isValidCard(discardPile_p, *head, i))
                    {   print_played_card(*head, i);                                // Print the card that the player wants to play
                        record_pre_card(discardPile_p, *head, i, isClockWise);      // Record the card played
                        remove_one_card(*head, i);                                  // Discard one card
                        sortCards(&((*head)->player.playerDeck));                   // Sort the cards to print later
                        print_player_cards(*head);                                  // Print the player's new cards
                        *breakLoop_p = endRound(head, current_r_p, demo_mode, fid); // Check if the player has no cards left
                        moveNext(head, isClockWise);return;}}}
            while (1) // Player-mode: Use a while loop to ensure that the player plays a valid card
            {   print_pre_card(*discardPile_p, demo_mode); // Print the previously played card
                print_player_cards(*head);                 // print the player's original cards
                printf("Please enter the serial number of the card you want to play: ");
                // Ensure the player enter one number
                while ((scanf("%d", &choice) != 1) || (choice > (*head)->player.playerDeck.cardArray_num) || (choice < 0)) // "choice" starts from 1, not 0
                {   printf("Wrong input! Please enter again!\n");
                    printf("Please enter the serial number of the card you want to play: ");
                    while (getchar() != '\n');} // Clear the buffer
                if (isValidCard(discardPile_p, *head, choice - 1))
                {   print_played_card(*head, choice - 1); // Print the card that the player wants to discard
                    print_played_card_log(fid, *head, choice - 1, demo_mode);
                    record_pre_card(discardPile_p, *head, choice - 1, isClockWise); // Record the card played
                    remove_one_card(*head, choice - 1);                             // Discard one card
                    print_player_cards(*head);                                      // Print the player's new cards
                    clear_screen(demo_mode);                                        // Clear the screen
                    *breakLoop_p = endRound(head, current_r_p, demo_mode, fid);     // Check if the player has no cards left
                    moveNext(head, isClockWise);return;}
                else
                {   printf("Invalid Card ! Please reenter !\n");}}}}

    else if (drawSeveralCards == 2) // The previous card is 2/3 and the player has cards ranked 2/3/7/Q
    {   if (demo_mode == 1) // The demo-mode
        {   shuffle(&((*head)->player.playerDeck));                            // Shuffle the cards to ensure a random card is played
            for (int i = 0; i < (*head)->player.playerDeck.cardArray_num; i++) // Traverse each card until a valid card is found
            {   if ((is_rank_played(*head, i, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Rank)) || (is_suit_rank_played(*head, i, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, TWO)) || (is_suit_rank_played(*head, i, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, THREE)) || (is_suit_rank_played(*head, i, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, SEVEN)) || (is_suit_rank_played(*head, i, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, JACK)) || (is_suit_rank_played(*head, i, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, QUEEN)))
                {   print_played_card(*head, i);
                    record_pre_card(discardPile_p, *head, i, isClockWise);
                    remove_one_card(*head, i);
                    sortCards(&((*head)->player.playerDeck));
                    print_player_cards(*head);
                    *breakLoop_p = endRound(head, current_r_p, demo_mode, fid);
                    moveNext(head, isClockWise);return;}}}
        while (1) // The player-mode
        {   print_pre_card(*discardPile_p, demo_mode);
            print_player_cards(*head);
            printf("Please enter the serial number of the card you want to play: ");
            while ((scanf("%d", &choice) != 1) || (choice > (*head)->player.playerDeck.cardArray_num) || (choice < 0)) // "choice" starts from 1, not 0
            {   printf("Wrong input! Please enter again!\n");
                printf("Please enter the serial number of the card you want to play: ");
                while (getchar() != '\n');} // Clear the buffer
            if ((is_rank_played(*head, choice - 1, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Rank)) || (is_suit_rank_played(*head, choice - 1, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, TWO)) || (is_suit_rank_played(*head, choice - 1, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, THREE)) || (is_suit_rank_played(*head, choice - 1, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, SEVEN)) || (is_suit_rank_played(*head, choice - 1, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, JACK)) || (is_suit_rank_played(*head, choice - 1, discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit, QUEEN))) // If the card is valid (2/3/7/Jack/Queen)
            {   print_played_card(*head, choice - 1);
                print_played_card_log(fid, *head, choice - 1, demo_mode);
                record_pre_card(discardPile_p, *head, choice - 1, isClockWise);
                remove_one_card(*head, choice - 1);
                print_player_cards(*head);
                clear_screen(demo_mode);
                *breakLoop_p = endRound(head, current_r_p, demo_mode, fid);
                moveNext(head, isClockWise);return;}
            else
            {   printf("Invalid Card ! Please reenter !\n");}}}}

// Play one round
void play_one_round(Deck *stockPile_p, DiscardPile *discardPile_p, int *breakLoop_p, int *current_r_p, int n, int r, FILE *fid, int demo_mode, Node **head, Node *headStable, int *isClockWise)
{
    while (1)
    {
        // play one card
        play_one_card(head, discardPile_p, stockPile_p, demo_mode, fid, isClockWise, breakLoop_p, current_r_p);
        print_PC_log(fid, *head, demo_mode);
        // end the game if the player has no cards in hand
        if ((*breakLoop_p) == 1)
        {
            break;
        }
    }
    // Find the first player
    *head = headStable;
    calculatePenalty(*head, n);
    // Print each player's score
    print_score(*head, n, *current_r_p);
    // Print each player's score in the log file
    print_score_log(fid, *head, n, *current_r_p, demo_mode);
    // Print the winner of the game when the last round is over
    if (*current_r_p == r)
    {
        winner(fid, *head, n, demo_mode);
        // Free allocated memory
        freeMemory(stockPile_p, discardPile_p, n, head);
    }
}
