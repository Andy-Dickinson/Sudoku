#include <stdio.h>
#include <math.h>

#define SIZE 9

// stored as global variables to avoid recalculating - will be constant for each game board size
int BOX_ROWS = floor(sqrt(SIZE)); // rows per box - 9x9 board will be 3, 6x6 will be 2
int BOX_COLS = ceil(sqrt(SIZE)); // cols per box - 9x9 board will be 3, 6x6 will be 3

void display_board(int grid[][SIZE]); // iterates through grid to print game board
void init(int grid[][SIZE]); // initialise grid
int get_box_num(int grid[][SIZE], int row, int col); // calculate box number (0 indexed)
void sepearator_row(); // prints a row of ---

// program starts here
int main() {

    // define grid
    int grid[SIZE][SIZE];

    // initialise grid
    init(grid);

    // print board
    display_board(grid);

    return 0;
}

// iterates through grid to print game board
void display_board(int grid[][SIZE]) {
    int row;
    int col;

    // indents top left corner of board (header row)
    printf("    ");

    // prints column headers
    for(int c=1; c<=SIZE; c++) {
        // converts int to char
        printf("%c ", c+64);

        // increases spacing for new box headers
        if((c)%BOX_COLS == 0) {
            printf("  ");
        }
    }
    printf("\n");

    sepearator_row();

    // iterates rows
    for(row=0; row<SIZE; row++) {

        // iterates columns
        for(int col=0; col<SIZE; col++) {

            // row ID
            if(col==0){
                if(row<9) {
                    printf(" %d| ", row+1);
                }else {
                    // double digit row ID
                    printf("%d| ", row+1);
                }
            }

            // print board values
            int value = grid[row][col];
            if(value == 0){
                printf("_ ");
            }else{
                printf("%d ", value);
            }

            // box separator
            if((col+1)%BOX_COLS == 0) {
                printf("| ");
            }
        }

        printf("\n");

        // inserts separator row between boxes
        if((row+1)%BOX_ROWS == 0) {
            sepearator_row();
        }
    }
}

// initialise grid
void init(int grid[][SIZE]) {
    int row;
    int col;

    // iterates through grid and setting each cell to 0
    for(row=0; row<SIZE; row++) {
        for(col=0; col<SIZE; col++){
            grid[row][col] = 0;
        }
    }
}

// calculate box number (0 indexed)
int get_box_num(int grid[][SIZE], int row, int col) {
    int box_num = ((int)row) / BOX_ROWS * BOX_ROWS + ((int)col) / BOX_COLS;
    return box_num;
}

// prints a separator row
void sepearator_row() {
    printf("---");
    // for varying grid size, amount of dashes required is double the number of columns per box + 2
    // then multiplied by number of boxes horizontally which coincides with the number of rows per box
    // plus 3 to cover row id column
    for(int i=0; i<(BOX_COLS*2+2)*BOX_ROWS; i++) {
        printf("-");
    }
    printf("\n");
}
