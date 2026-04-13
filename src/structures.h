#ifndef STRUCTURES
#define STRUCTURES

// Rank
#define TWO 0
#define THREE 1
#define FOUR 2
#define FIVE 3
#define SIX 4
#define SEVEN 5
#define EIGHT 6
#define NINE 7
#define TEN 8
#define JACK 9
#define QUEEN 10
#define KING 11
#define ACE 12

// Suit
#define SPADES 0
#define HEARTS 1
#define DIAMONDS 2
#define CLUBS 3

// Define a structure of Card
typedef struct
{
    int Suit; // Suit
    int Rank; // Rank
} Card;

// Define a structure of Deck
typedef struct
{
    // array of cards
    Card *cardArray;

    // number of cards in the array
    int cardArray_num;

} Deck;

// Define a structure of DiscardPile
typedef struct
{
    // decks of cards
    Deck discardDeck;

    // record the special function
    int *ToPlus;
    int *ToSkip;

} DiscardPile;

// Define a structure of player
typedef struct
{
    Deck playerDeck; // Player's deck
    int score;       // Player's total score
} Player;

// Define a structure of Node
typedef struct _Node
{
    // Data
    Player player;

    // Index
    int ind;

    // Pointer to the previous Node
    struct _Node *pre;

    // Pointer to the next Node
    struct _Node *nxt;
} Node;

#endif
