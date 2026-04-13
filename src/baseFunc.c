#include <stdio.h>
#include <stdlib.h>
#include "baseFunc.h"

// Initialize a node
void initNode(Node **head, int c, FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode)
{   *head = (Node *)malloc(sizeof(Node));
    if (*head == NULL)
    {   fprintf(stderr, "Memory allocation failed !\n");
        exit(-1);}
    (*head)->pre = *head; (*head)->nxt = *head; (*head)->ind = 0;
    Player p; p.score = 0; p.playerDeck.cardArray_num = 0; p.playerDeck.cardArray = NULL;
    (*head)->player = p;
    for (int i = 0; i < c; i++)
    {   draw_one_card(fid, *head, stockPile_p, discardPile_p, demo_mode);}}
// Add a node
void addNode(Node **head, int c, FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode)
{   Node *append = NULL; // Create a node
    initNode(&append, c, fid, stockPile_p, discardPile_p, demo_mode);
    Node *last = *head; // Search for the last element in Node **head
    while (last->nxt != *head)
    {   last = last->nxt;}
    append->ind = last->ind + 1; // The index of the appended node
    last->nxt = append;          // Let the "nxt" pointer of the last element in node_t **head point to the newNode
    append->pre = last;          // Let the "pre" pointer of the newNode point to the last element in node_t **head
    append->nxt = *head;         // Let the "nxt" pointer of the newNode point to the first element in node_t **head
    (*head)->pre = append;}       // Let the "pre" pointer of the the first element in node_t **head point to newNode
// Renew the player data in the node after a round
void renewList(Node *head, int n, int c, FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode)
{   Node *current = head;
    for (int i = 0; i < n; i++)
    {   current->player.playerDeck.cardArray_num = 0;
        for (int j = 0; j < c; j++)
        {   draw_one_card(fid, current, stockPile_p, discardPile_p, demo_mode);}
        current = current->nxt;}}
// Decide which player plays first
int firstPlayer_initCard(int playerNum, Deck *stockPile_p, DiscardPile *discardPile_p, Card *initialCard_p, Node **head)
{   int first_player_index = -1;
    int first_player_card_rank = 13; // ACE = 12
    // The player with the smallest card rank plays first. If the ranks are the same, then we follow both the smallest rank and the smallest Player ID (Player 1 < Player 2 < Player 3)
    for (int i = 0; i < playerNum; i++)
    {   if (stockPile_p->cardArray[i].Rank < first_player_card_rank)
        {   first_player_card_rank = stockPile_p->cardArray[i].Rank;
            first_player_index = i;}}
    *initialCard_p = stockPile_p->cardArray[playerNum]; // The card to initialize the game
    for (int i = 0; i <= playerNum; i++)
    {   increase_discard_pile_space(discardPile_p); // enlarge the discardPile
        discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1] = stockPile_p->cardArray[0]; // set the values for the new discardPile
        discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1] = 0;
        discardPile_p->ToSkip[discardPile_p->discardDeck.cardArray_num - 1] = 0;
        reduce_stock_pile(stockPile_p);} // reduce the stockPile
    Node *firstPlayer = *head;           // Renew the Node *head according to the first player index
    for (int i = 0; i < playerNum; i++)
    {   if (firstPlayer->ind == first_player_index)
        {   *head = firstPlayer;
            break;}
        firstPlayer = firstPlayer->nxt;}
    return first_player_index;}
// Shuffle a pile
void shuffle(Deck *Pile_p)
{   int len = Pile_p->cardArray_num;
    for (int i = 0; i < len - 1; i++) // from the first to the second-to-last element
    {   int swapPosition = i + rand() % (len - i); // randomly choose a position between the current position and the last position
        Card temp = Pile_p->cardArray[swapPosition]; // swap the current position and the position chosen
        Pile_p->cardArray[swapPosition] = Pile_p->cardArray[i];
        Pile_p->cardArray[i] = temp;}}
// Judge if there is no card left in both SP and DP
int able_to_draw(FILE *fid, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode)
{
    if ((stockPile_p->cardArray_num == 0) && (discardPile_p->discardDeck.cardArray_num == 1))
    {   printf("\nDrawing cards stops. Stock pile and discard pile exhausted.\n");
        if (demo_mode == 0)
        {   printLog(fid, "\nDrawing cards stops. Stock pile and discard pile exhausted.\n");}
        return 0;}
    else
    {    return 1;}}
// Draw a card
void draw_one_card(FILE *fid, Node *head, Deck *stockPile_p, DiscardPile *discardPile_p, int demo_mode)
{
    head->player.playerDeck.cardArray_num++; // add the new card to the original card array
    Card *temp = (Card *)realloc(head->player.playerDeck.cardArray, ((size_t)(head->player.playerDeck.cardArray_num)) * sizeof(Card));
    if (temp == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n"); // Handle malloc failure
        exit(EXIT_FAILURE);
    }
    head->player.playerDeck.cardArray = temp;
    restore_stock_pile(fid, discardPile_p, stockPile_p, demo_mode);                                           // Restore the SP and renew the DP when necessary
    head->player.playerDeck.cardArray[head->player.playerDeck.cardArray_num - 1] = stockPile_p->cardArray[0]; // Draw a card : add the first card of the shuffled stock pile
    reduce_stock_pile(stockPile_p);
} // Reduce the stock pile
// Judge if the player has card ranked n (eg: 2/3/7/J/Q)
int is_rank_contained(Node *head, int rank)
{   for (int i = 0; i < head->player.playerDeck.cardArray_num; i++)
    {   if (head->player.playerDeck.cardArray[i].Rank == rank)
        {   return 1;}}
    return 0;}
// Judge if the player has played a card ranked n (eg: 2/3/7/J/Q)
int is_rank_played(Node *head, int remove_position, int n)
{   if (head->player.playerDeck.cardArray[remove_position].Rank == n)
    {   return 1;}
    return 0;}
// Judge if the player has cards that have Suit s and Rank n
int is_suit_rank_contained(Node *head, int suit, int rank)
{   for (int i = 0; i < head->player.playerDeck.cardArray_num; i++)
    {   if ((head->player.playerDeck.cardArray[i].Suit == suit) && (head->player.playerDeck.cardArray[i].Rank == rank))
        {   return 1;}}
    return 0;}

// Judge if the player plays a card that has Suit s and Rank n
int is_suit_rank_played(Node *head, int remove_position, int suit, int rank)
{   if ((head->player.playerDeck.cardArray[remove_position].Suit == suit) && (head->player.playerDeck.cardArray[remove_position].Rank == rank))
    {   return 1;}
    return 0;}
// Judge if the card played is valid
int isValidCard(DiscardPile *discardPile_p, Node *head, int remove_position)
{   if ((discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Rank != head->player.playerDeck.cardArray[remove_position].Rank) && (discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Suit != head->player.playerDeck.cardArray[remove_position].Suit)) // The card can't be played if its suit or rank isn't the same as that of the previous played card
    {   return 0;} return 1;}
// Judge if all the card a player has is invalid
int is_all_invalid(DiscardPile *discardPile_p, Node *head)
{   for (int i = 0; i < head->player.playerDeck.cardArray_num; i++)
    {   if (isValidCard(discardPile_p, head, i) == 1)
        {   return 0;}}
    return -1;} // We return -1 in order not mistake it for the index
// Record the card played
void record_pre_card(DiscardPile *discardPile_p, Node *head, int remove_position, int *isClockWise)
{   discardPile_p->discardDeck.cardArray_num++; // add the times
    Card *temp1 = (Card *)realloc(discardPile_p->discardDeck.cardArray, ((size_t)discardPile_p->discardDeck.cardArray_num) * sizeof(Card)); // reassign the memory for discardPile_p->discardDeck.cardArray (Card *)
    if (temp1 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n"); // Handle realloc failure
        exit(EXIT_FAILURE);}
    discardPile_p->discardDeck.cardArray = temp1;
    discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1] = head->player.playerDeck.cardArray[remove_position]; // record the current card as previous card
    int *temp2 = (int *)realloc(discardPile_p->ToPlus, ((size_t)discardPile_p->discardDeck.cardArray_num) * sizeof(int)); // Plus
    if (temp2 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);}
    discardPile_p->ToPlus = temp2;
    // if the last card played ranks 2 or 3, then we add up the plusTimes and give it to "To_Plus" (last), and set the "To_Plus" array (second to last) to 0, meaning the "plus" effect has been used
    if (discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1].Rank == TWO)
    {   discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1] = discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2] + 2;
        discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2] = 0;}
    else if (discardPile_p->discardDeck.cardArray[(discardPile_p->discardDeck.cardArray_num) - 1].Rank == THREE)
    {   discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1] = discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2] + 3;
        discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2] = 0;}
    else if (discardPile_p->discardDeck.cardArray[(discardPile_p->discardDeck.cardArray_num) - 1].Rank == JACK) // if the last card played ranks JACK, then we shift the Used_To_Plus rightwards
    {   discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1] = discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2];
        discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2] = 0;}
    else if (discardPile_p->discardDeck.cardArray[(discardPile_p->discardDeck.cardArray_num) - 1].Rank == QUEEN) // if the last card played ranks QUEEN, then we shift the Used_To_Plus rightwards and renew the sequence
    {   discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1] = discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2];
        discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2] = 0; (*isClockWise)++;}
    else // if the last card played ranks 7 or else
    {   discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 1] = 0;
        discardPile_p->ToPlus[discardPile_p->discardDeck.cardArray_num - 2] = 0;}
    int *temp3 = (int *)realloc(discardPile_p->ToSkip, ((size_t)discardPile_p->discardDeck.cardArray_num) * sizeof(int)); // Skip
    if (temp3 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);}
    discardPile_p->ToSkip = temp3;
    if (discardPile_p->discardDeck.cardArray[(discardPile_p->discardDeck.cardArray_num) - 1].Rank == JACK) // If the previous card played is "Jack", then the next player should be skipped
    {   discardPile_p->ToSkip[discardPile_p->discardDeck.cardArray_num - 1] = 1;} // the "skip" effect exists
    else // If the previous card played isn't "Jack", then the next player should not be skipped
    {   discardPile_p->ToSkip[discardPile_p->discardDeck.cardArray_num - 1] = 0;}} // the "skip" effect doesn't exist
// Reduce the stock pile
void reduce_stock_pile(Deck *stockPile_p)
{   for (int i = 0; i < stockPile_p->cardArray_num - 1; i++)
    {
        // delete one card from the stock pile
        stockPile_p->cardArray[i] = stockPile_p->cardArray[i + 1];
    }
    stockPile_p->cardArray_num--;} // decrease the number of cards by 1

// Remove one card
void remove_one_card(Node *head, int remove_position)
{   for (int i = remove_position; i < head->player.playerDeck.cardArray_num - 1; i++)
    {   head->player.playerDeck.cardArray[i] = head->player.playerDeck.cardArray[i + 1];}
    head->player.playerDeck.cardArray_num--; // subtract the number of cards by 1
    if (head->player.playerDeck.cardArray_num == 0) // Without this, when a player's card number equals to 0, head->player.playerDeck.cardArray will be NULL
    {   return;}
    head->player.playerDeck.cardArray = realloc(head->player.playerDeck.cardArray, ((size_t)head->player.playerDeck.cardArray_num) * sizeof(Card));
    if (head->player.playerDeck.cardArray == NULL)
    {   fprintf(stderr, "Memory allocation failed\n"); // Handle realloc failure
        exit(EXIT_FAILURE);}}
// Determine "clockwise" or "counterclockwise" and jump to the next player
void moveNext(Node **head, const int *isClockWise)
{   if (((*isClockWise) % 2) == 1) // Clockwise
    {   *head = (*head)->nxt;}
    else // Counter-clockwise
    {   *head = (*head)->pre;}}
// Increase the space of the discard pile (without setting values)
void increase_discard_pile_space(DiscardPile *discardPile_p)
{   discardPile_p->discardDeck.cardArray_num++; // add the times
    Card *temp1 = (Card *)realloc(discardPile_p->discardDeck.cardArray, ((size_t)discardPile_p->discardDeck.cardArray_num) * sizeof(Card)); // reassign the memory for discardPile_p->discardDeck.cardArray (Card *)
    if (temp1 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n"); // Handle realloc failure
        exit(EXIT_FAILURE);}
    discardPile_p->discardDeck.cardArray = temp1;
    int *temp2 = (int *)realloc(discardPile_p->ToPlus, ((size_t)discardPile_p->discardDeck.cardArray_num) * sizeof(int)); // reassign the memory for discardPile_p->ToPlus (int*)
    if (temp2 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);}
    discardPile_p->ToPlus = temp2;
    int *temp3 = (int *)realloc(discardPile_p->ToSkip, ((size_t)discardPile_p->discardDeck.cardArray_num) * sizeof(int)); // reassign the memory for discardPile_p->ToSkip (int*)
    if (temp3 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);}
    discardPile_p->ToSkip = temp3;}
// Renew the discard pile. The only card in the new DP is the last card of the old DP
void renew_discard_pile(DiscardPile *discardPile_p)
{   Card tempCard = discardPile_p->discardDeck.cardArray[discardPile_p->discardDeck.cardArray_num - 1]; // record the last card of the DP
    discardPile_p->discardDeck.cardArray_num = 1; // renew the card number of DP
    Card *temp1 = (Card *)realloc(discardPile_p->discardDeck.cardArray, sizeof(Card)); // reassign the memory for discardPile_p->discardDeck.cardArray (Card *)
    if (temp1 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n"); // Handle realloc failure
        exit(EXIT_FAILURE);}
    discardPile_p->discardDeck.cardArray = temp1;
    discardPile_p->discardDeck.cardArray[0] = tempCard;
    int *temp2 = (int *)realloc(discardPile_p->ToPlus, sizeof(int)); // reassign the memory for discardPile_p->ToPlus (int*)
    if (temp2 == NULL)
    {    fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);}
    discardPile_p->ToPlus = temp2;
    discardPile_p->ToPlus[0] = 0;
    int *temp3 = (int *)realloc(discardPile_p->ToSkip, sizeof(int)); // reassign the memory for discardPile_p->ToSkip (int*)
    if (temp3 == NULL)
    {   fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);}
    discardPile_p->ToSkip = temp3;
    discardPile_p->ToSkip[0] = 0;}
// Restore the stock pile if it has no cards left
void restore_stock_pile(FILE *fid, DiscardPile *discardPile_p, Deck *stockPile_p, int demo_mode)
{   if (stockPile_p->cardArray_num == 0) // Restore the stock pile if and only if its card number is 0 and players need to draw cards
    {   printf("\nStockpile exhausted. Shuffling the discard pile and restore the stock pile\n\n"); // Print the shuffling status
        if (demo_mode == 0)
        {   printLog(fid, "\nStockpile exhausted. Shuffling the discard pile and restore the stock pile\n\n");}
        stockPile_p->cardArray_num = discardPile_p->discardDeck.cardArray_num - 1; // Copy discardPile_p (n-1) to stockPile_p. Renew the stockPile_p->cardArray_num
        Card *temp = realloc(stockPile_p->cardArray, ((size_t)stockPile_p->cardArray_num) * sizeof(Card)); // Reassign the memory for the stock pile
        if (temp == NULL)
        {   fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);}
        stockPile_p->cardArray = temp;
        for (int i = 0; i < stockPile_p->cardArray_num; i++) // Add the disCard pile to the stock pile
        {   stockPile_p->cardArray[i] = discardPile_p->discardDeck.cardArray[i];}
        shuffle(stockPile_p); // Shuffle the stock pile
        renew_discard_pile(discardPile_p);}} // Renew the discard pile
// End the round if a player has no cards in hand
int endRound(Node **head, int *current_r_p, int demo_mode, FILE *fid)
{   // end the game if the player has no cards in hand
    if ((*head)->player.playerDeck.cardArray_num == 0)
    {   printf("\nRound %d Winner: Player %d\n", *current_r_p, (*head)->ind + 1); // Print the winner of the round
        if (demo_mode == 0)
        {   printLog(fid, "\nRound %d Winner: Player %d\n", *current_r_p, (*head)->ind + 1);}
        return 1;} // End the round
    return 0;} // Proceed the round
// Free a list of nodes
void freeList(Node **head, int n)
{   Node *current = *head;
    Node *next = NULL;
    for (int i = 0; i < n; i++)
    {   next = current->nxt;
        if (current->player.playerDeck.cardArray != NULL)
        {   free(current->player.playerDeck.cardArray);}
        if (current != NULL)
        {   free(current);}
        current = next;}
    *head = NULL;}
// Calculate the penalty of each round and add them up to the player's score
void calculatePenalty(Node *head, int n)
{   Node *current = head;
    for (int i = 0; i < n; i++)
    {   current->player.score -= current->player.playerDeck.cardArray_num;
        current = current->nxt;}}
// Determine who wins eventually (Maybe more than one winner)
void winner(FILE *fid, Node *head, int n, int demo_mode)
{   int maxScore = head->player.score; // Suppose the first player has the max score
    Node *current = head;
    for (int i = 0; i < n; i++)
    {   if (current->player.score > maxScore)
        {   maxScore = current->player.score;}
        current = current->nxt;}
    for (int i = 0; i < n; i++)
    {   if (current->player.score == maxScore)
        {   printf("\nPlayer %d wins!", current->ind + 1);
            if (demo_mode == 0)
            {   printLog(fid, "\nPlayer %d wins!\n", current->ind + 1);}}
        current = current->nxt;}
    printf("\n\n");}
// Free allocated memory
void freeMemory(Deck *stockPile_p, DiscardPile *discardPile_p, int n, Node **head)
{   if (stockPile_p->cardArray != NULL)
    {   free(stockPile_p->cardArray);} // Free stock pile
    if (discardPile_p->discardDeck.cardArray != NULL)
    {   free(discardPile_p->discardDeck.cardArray);} // Free discard pile
    if (discardPile_p->ToPlus != NULL)
    {   free(discardPile_p->ToPlus);}
    if (discardPile_p->ToSkip != NULL)
    {   free(discardPile_p->ToSkip);}
    freeList(head, n);} // Free node
