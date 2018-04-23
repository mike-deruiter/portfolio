/*******************************************************************************
 * montyhall - Simulates the "Monty Hall Problem", based on the old TV show 
 * "Let's Make A Deal". There are x number of doors (traditionally 3). Behind 
 * one is a shiny red sports car, and behind all the rest are lumps of coal. 
 * The player guesses which door is the one with the sports car behind it. 
 * Monty Hall then opens all the remaining doors except one. Because Monty Hall 
 * is like Santa Claus & knows everything, the doors he opens always reveal 
 * lumps of coal. The player must then decide whether to switch to the remaining
 * door or stay with their original choice. With only three doors, this seems 
 * like a true dilemma. The question is, is there a definitive strategy to 
 * maximize your chances of winning? The answer is yes; you should ALWAYS switch
 * doors. This is easily intuited when the number of doors is large. This 
 * program simulates a computer playing the game against itself an arbitrary 
 * number of times, once while following the strategy, and once while not, with
 * an arbitrary number of doors. It demonstrates that the probability of winning
 * when following the strategy is (x-1)/x while the probability of winning when
 * not following the strategy is merely 1/2.
 * 
 * Coded by Mike DeRuiter.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DOORS       3
#define ITERATIONS  10000

bool playgame(bool strategy);
void init_doors();

bool *doors;

int main()
{
    int i;

    int winsStrategy, winsNoStrategy;
    double winsStratPercent, winsNoStratPercent;

    winsStrategy = winsNoStrategy = 0;
    
    srand(time(NULL));

    doors = (bool *) malloc(DOORS * sizeof(bool));

    // play the game 'iterations' times following the strategy.
    for (i = 0; i < ITERATIONS; ++i)
        if (playgame(true))
            ++winsStrategy;

    // play the game 'interations' times not following the strategy.
    for (i = 0; i < ITERATIONS; ++i)
        if (playgame(false))
            ++winsNoStrategy;

    /* calculate the percentages & convert to the form normally understood
       by humans.                                                          */
    winsStratPercent = (double) winsStrategy / (double) ITERATIONS;
    winsNoStratPercent = (double) winsNoStrategy / (double) ITERATIONS;
    
    winsStratPercent *= 100;
    winsNoStratPercent *= 100;

    // print output
    printf("The computer won %6.2f%% of the time while following the "
           "strategy.\n", winsStratPercent);
    printf("The computer won %6.2f%% of the time while not following the "
           "strategy.\n", winsNoStratPercent);

    return 0;
}

void init_doors() {
    int i;

    for (i = 0; i < DOORS; ++i)
        doors[i] = false;
}

/* playgame - play a single round of the game. */
bool playgame(bool followingStrategy)
{
    int i;

    init_doors();

    int winningDoor = rand() % DOORS;
    doors[winningDoor] = true;

    // the player guesses a random door.
    int playerGuess = rand() % DOORS;
 
    int closedDoor;
    
    /* the closed door is the winning door unless the player's guess is
       already the winning door, in which case it's random              */
    if (winningDoor != playerGuess)
        closedDoor = winningDoor;
    else
        do {
            closedDoor = rand() % DOORS;
        } while (closedDoor == playerGuess);

    /* if the player is following the strategy, switch their guess to the
     * closed door                                                        */ 
    if (followingStrategy)
        playerGuess = closedDoor;
    else
        playerGuess = (rand() % 2) ? playerGuess : closedDoor;

    // return whether or not the player won.
    if (doors[playerGuess]) {
        doors[playerGuess] = false;
        return true;
    } else {
        doors[winningDoor] = false;
        return false;
    }
 }
