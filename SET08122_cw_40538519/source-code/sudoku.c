#include "sudoku.h"
#include "user_interface.h"

// global variables
int SIZE;
bool timer_setting;
bool colour_aid_setting;
clock_t timer;
int **solution_grid;
int **player_grid;
int **original_player_grid;
int BOX_ROWS; // rows per box - 9x9 board will be 3, 6x6 will be 2
int BOX_COLS; // cols per box - 9x9 board will be 3, 6x6 will be 3
int num_to_remove;
bool solved;
int grid_empties;

int main(void) {

    menu();

    printf("Quit game\n\n");

    return (0);
}
