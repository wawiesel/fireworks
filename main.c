/**
 * main.c
 *
 * Author: Michael Stegeman
 * Date: 2/25/08
 *
 * Description: ASCII art fireworks demo
 */

#include "particle.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

/**
 * initializes necessary ncurses attributes
 */
void ncurses_init() {
    WINDOW *window = initscr(); // initialize screen
    noecho();                   // don't display typed characters
    cbreak();                   // don't wait until RETURN is pressed
    nodelay(window, TRUE);      // don't wait for key press
    curs_set(0);                // make cursor invisible
    start_color();              // start color in terminal
    init_pair(1, 7, 0);         // White on black
    init_pair(2, 6, 0);         // Cyan on black
    init_pair(3, 5, 0);         // Magenta on black
    init_pair(4, 4, 0);         // Blue on black
    init_pair(5, 3, 0);         // Yellow on black
    init_pair(6, 2, 0);         // Green on black
    init_pair(7, 1, 0);         // Red on black
}

int main() {
    struct timespec interval = { .tv_sec = 0, .tv_nsec = 25000000 };
    size_t size = 200;

    // Experiment with these :).
    float speed = 0.025; // max speed of firework
    float vary = 0.8; //variability factor between 0 and 1 (1 is max)
    size_t nfire=20; // number of fireworks
    
    ncurses_init();
    srand((unsigned) time(NULL));

    particle* array[nfire];
    for(size_t i=0; i<nfire; ++i)
    {
        // reserve memory for particle array
        array[i] = (particle *) calloc(sizeof(particle), size);
        // initialize particle array
        particle_init(array[i], size);
    }

    // check for user input to quit program
    while (getch() != 'q') {
    
        for(size_t i=0; i<nfire; ++i)
        {
            particle* p = array[i];
            for(size_t j=i+1; j<nfire-3; ++j)
            {
                if (array[i][0].life < 1.21 && array[i][0].life > 1.19) {
                    // reinitialize second array at 3/4 life of first
                   if( array[j+1][0].life < 0.01 ) particle_init(array[j+1], size);
                } else if (array[i+1][0].life < 0.81 && array[i+1][0].life > 0.79) {
                    // reinitialize third array at 1/2 life of first
                    if( array[j+2][0].life < 0.01 ) particle_init(array[j+2], size);
                } else if (array[i+2][0].life < 0.41 && array[i+2][0].life > 0.39) {
                    // reinitialize fourth array at 1/4 life of first
                    if( array[j+3][0].life < 0.01 ) particle_init(array[j+3], size);
                }
            }
            if (array[i][0].life < 0.01) {
                // check for end of life and reinitialize array if dead
                particle_init(p, size);
            }
        }
        // erase screen
        erase();

        // update all particles
        for(size_t i=0; i<nfire; ++i)
        {
            float x = (float)rand() / (float)RAND_MAX;
            particle_update(array[i], speed*(vary*x+1.0-vary), size);
        }

        // draw all particles to screen
        for(size_t i=0; i<nfire; ++i)
        {
            particle_draw(array[i], size);
        }

        // draw particles to screen
        refresh();

        // wait a bit
        nanosleep(&interval, NULL);
    }

    for(size_t i=0; i<nfire; ++i)
    {
        free(array[i]);
        array[i]=NULL;
    }

    endwin();

    return 0;
}
