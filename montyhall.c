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

// if we're using c99 or later, include stdbool.h; otherwise, roll our own
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef enum { false, true } bool;
#endif

#define DOORS       3
#define ITERATIONS  10000

bool playgame(bool strategy);
void init_doors();

bool doors[DOORS];

int main()
{
    int i;

    int wins_strat, wins_nostrat;
    double strat_percent, nostrat_percent;

    wins_strat = wins_nostrat = 0;

    // seed the random number generator    
    srand(time(NULL));

    // play the game 'iterations' times following the strategy.
    for (i = 0; i < ITERATIONS; ++i)
        if (playgame(true))
            ++wins_strat;

    // play the game 'interations' times not following the strategy.
    for (i = 0; i < ITERATIONS; ++i)
        if (playgame(false))
            ++wins_nostrat;

    /* calculate the percentages & convert to the form normally understood
       by humans.                                                          */
    strat_percent = (double) wins_strat / (double) ITERATIONS;
    nostrat_percent = (double) wins_nostrat / (double) ITERATIONS;
    
    strat_percent *= 100;
    nostrat_percent *= 100;

    // print output
    printf("The computer won %.2f%% of the time while following the "
           "strategy.\n", strat_percent);
    printf("The computer won %.2f%% of the time while not following the "
           "strategy.\n", nostrat_percent);

    return 0;
}

void init_doors() {
    int i;

    for (i = 0; i < DOORS; ++i)
        doors[i] = false;
}

/* playgame - play a single round of the game. */
bool playgame(bool following_strat)
{
    int i;

    init_doors();

    int winning_door = rand() % DOORS;
    doors[winning_door] = true;

    // the player guesses a random door.
    int player_guess = rand() % DOORS;
 
    int closed;
    
    /* the closed door is the winning door unless the player's guess is
       already the winning door, in which case it's random              */
    if (winning_door != player_guess)
        closed = winning_door;
    else
        do {
            closed = rand() % DOORS;
        } while (closed == player_guess);

    /* if the player is following the strategy, switch their guess to the
     * closed door                                                        */ 
    if (following_strat)
        player_guess = closed;
    else
        player_guess = (rand() % 2) ? player_guess : closed;

    // return whether or not the player won.
    if (doors[player_guess]) {
        doors[player_guess] = false;
        return true;
    } else {
        doors[winning_door] = false;
        return false;
    }
 }
