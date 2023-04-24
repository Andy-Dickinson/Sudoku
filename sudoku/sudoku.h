#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include<dirent.h>

extern int SIZE;
extern bool timer_setting;
extern bool colour_aid_setting;
extern clock_t timer;
extern int **solution_grid;
extern int **player_grid;
extern int **original_player_grid;
extern int BOX_ROWS; // rows per box - 9x9 board will be 3, 6x6 will be 2
extern int BOX_COLS; // cols per box - 9x9 board will be 3, 6x6 will be 3
extern int num_to_remove;
extern bool solved;
extern int grid_empties;

#endif // SUDOKU_H
