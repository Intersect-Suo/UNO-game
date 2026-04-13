#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "compFunc.h"

int main(int argc, char **argv)
{   // Default options
    char logName[30] = "onecard.log"; // The name of the log file
    int r = 1;                        // The round of games
    int d = 2;                        // The number of a complete deck of cards
    int n = 4;                        // The number of players
    int c = 5;                        // The initial number of players' cards
    int demo_mode = 0;                // Whether to use demo mode

    for (int i = 1; i < argc; i++)    // Parse the command line
    {   if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {   print_help();
            return 0;}
        else if (strcmp(argv[i], "--log") == 0 && ((i + 1) < argc))
        {   logName[0] = '\0';
            int result = snprintf(logName, sizeof(logName), "%s", argv[i + 1]);
            if (result < 0) // Check the return value of snprintf
            {   printf("Failed to format the name of the log file\n");
                exit(1);}
            else if (result >= (int)sizeof(logName))
            {   printf("The name of the log file you generated is too long\n");
                exit(1);}
            else
            {   i++;}} // Skip the next parameter, since it is the name of the log file
        else if ((strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--player-number=") == 0) && ((i + 1) < argc))
        {   int temp_n = atoi(argv[i + 1]);
            if (temp_n > 2)
            {    n = temp_n;}
            else
            {   fprintf(stderr, "Error: Player number must be larger than 2.\n");
                return 1;}
            i++;} // Skip the next parameter, since it is the number of players
        else if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--initial-cards=") == 0) && ((i + 1) < argc))
        {   int temp_c = atoi(argv[i + 1]);
            if (temp_c >= 2)
            {    c = temp_c;}
            else
            {   fprintf(stderr, "Error: Initial cards per player must be at least 2.\n");
                return 1;}
            i++;} // Skip the next parameter, since it is the initial number of cards each player has
        else if ((strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decks=") == 0) && ((i + 1) < argc))
        {   int temp_d = atoi(argv[i + 1]);
            if (temp_d >= 2)
            {   d = temp_d;}
            else
            {   fprintf(stderr, "Error: Number of decks must be at least 2.\n");
                return 1;}
            i++;} // Skip the next parameter, since it is the number of decks used
        else if ((strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--rounds=") == 0) && ((i + 1) < argc))
        {   int temp_r = atoi(argv[i + 1]);
            if (temp_r >= 1)
            {    r = temp_r;}
            else
            {   fprintf(stderr, "Error: Number of rounds must be at least 1.\n");
                return 1;}
            i++;} // Skip the next parameter, since it is the number of rounds of the game
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--auto") == 0)
        {   demo_mode = 1;}
        else
        {   fprintf(stderr, "Error: Unrecognized option '%s'.\n", argv[i]);
            print_help();
            return 1;}}
    if ((n + 1 + n * c) > d * 52) // Ensure the stock pile isn't exhausted before the game starts
    {   fprintf(stderr, "Error: expected number of cards exceeds total number of cards\n");
        fprintf(stderr, "Error: (n + 1 + n * c) > d * 52\n");
        exit(1);}
    FILE *fid = NULL;   // Create a log file
    if (demo_mode == 0) // Open the log file if demo_mode = 0
    {   fid = fopen(logName, "w");
        if (fid == NULL)
        {   fprintf(stderr, "Failed to open the log file !\n");
            exit(1);}}
    print_game_setting(r, n, c, d);                     // Print the game setting
    print_game_setting_log(fid, r, n, c, d, demo_mode); // Print the game setting in the log file

    srand((unsigned int)time(NULL));                    // Generate random numbers
    Deck stockpile;                                     // Create the stock pile
    DiscardPile discardPile;                            // Create the discard pile
    Card initialCard;                                   // Create the card to initialize the game
    int first_player_index = 0;                         // Create the index of the first player
    Node *head = NULL;                                  // Create a node
    Node *headStable = head;                            // Create another node to record the first player
    int breakLoop = 0;                                  // Create a flag to judge if the game is over
    int current_r = 0;                                  // Create a flag to record current round
    int isClockWise = 0;                                // Create a flag to record "clockwise" or "counterclockwise"
    for (int i = 0; i < r; i++)                         // Play r rounds
    {   initialize(&stockpile, &discardPile, &first_player_index, &initialCard, &breakLoop, &current_r, n, c, d, demo_mode, fid, &head, &headStable, &isClockWise); // Initialize
        play_one_round(&stockpile, &discardPile, &breakLoop, &current_r, n, r, fid, demo_mode, &head, headStable, &isClockWise);}                                  // Play one round
    if (demo_mode == 0) // Close the log file if demo_mode = 0
    {   if (fid != NULL)
        {   fclose(fid);}}
    return 0;}
