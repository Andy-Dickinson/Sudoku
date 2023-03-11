#include <stdio.h>
#include <math.h>
#include <time.h>

#define SIZE 12

// used for circular doubly linked list of all possible numbers to be entered in a box when partially completing grid
typedef struct node {
    int data;
    struct node* next;
    struct node* prev;
}node ;


// stored as global variables to avoid recalculating - will be constant for each game board size
int BOX_ROWS = floor(sqrt(SIZE)); // rows per box - 9x9 board will be 3, 6x6 will be 2
int BOX_COLS = ceil(sqrt(SIZE)); // cols per box - 9x9 board will be 3, 6x6 will be 3

void display_board(int** grid); // iterates through grid to print game board
void init(int** grid); // initialise grid
void sepearator_row(); // prints a row of ---
void partially_complete(int** grid); // randomly fills boxes which do not conflict with each other
node* create_box_num_list(); // creates doubly linked circular list of all numbers for a box
void clean_up(int** grid);


//////////////////////// currently not used
int get_box_num(int** grid, int row, int col); // calculate box number (0 indexed)


// program starts here
int main() {

    // declare grid
    int** grid;

    // allocate memory for grid
    // memory allocation code adapted from (No name or date, Multidimensional arrays in C, https://www.uio.no/studier/emner/matnat/ifi/IN3200/v19/teaching-material/multidimarrays.pdf)
    grid = malloc(SIZE * sizeof *grid);
    grid[0] = malloc(SIZE*SIZE * sizeof *grid[0]);

    for(int i=1; i<SIZE; i++) {
        grid[i] = &(grid[0][i*SIZE]);
    }

    // initialise grid
    init(grid);

    // randomly fills boxes which do not conflict with each other
    partially_complete(grid);

    // print board
    display_board(grid);

    clean_up(grid);

    return 0;
}

// deallocates memory
void clean_up(int** grid){
    free(grid[0]);
    free(grid);
}

// creates doubly linked circular list of all numbers for a box
node* create_box_num_list() {
    // creates new list
    node* list, * last, *  current;
    list = NULL;

    if(SIZE>0) {
        // allocates memory for first node
        list = (node*) malloc(sizeof(node));

        // creates first node
        list -> prev = list;
        list -> data = 1;
        list -> next = list;

        // tracks last node created
        last = (node*) malloc(sizeof(node));
        last = list;

        // creates new node for each number in a box and appends to list
        for(int i=2; i<=SIZE; i++) {
            // allocates memory for each new node
            current = (node*) malloc(sizeof(node));

            // inserts data to new node and sets prev to point as last and sets next to point back at first node
            current -> prev = last;
            current -> data = i;
            current -> next = list;

            // sets last node created to point at new node
            last -> next =  current;

            // updates last node tracker
            last =  current;
        }
    }

    return list;
}

// randomly fills boxes which do not conflict with each other
void partially_complete(int** grid) {
    int row = 0;
    int col = 0;
    node* box_numbers, * temp;

    // sets seed for random number generator
    srand(time(0));

    // randomly fills non conflicting boxes
    // iterates boxes diagonally down and to the right
    for(int box=0; box<SIZE/BOX_COLS; box++) {

        // generates doubly linked circular list of all numbers for a box
        // memory is allocated in method called
        box_numbers = create_box_num_list();

        int box_num_remaining = SIZE;

        // iterates rows in a box
        for(int i=0; i<BOX_ROWS; i++) {
            // sets column index to beginning of next box to the right
            col = BOX_COLS*box;
            // iterates columns in a box
            for(int j=0; j<BOX_COLS; j++) {
                // generate random number between 0 and number of box_num_remaining-1
                int num_pointer = rand() % (box_num_remaining);

                // loops pointer to list of remaining numbers to be placed, essentially selecting a random number from those remaining
                for(int i=0; i<num_pointer; i++){
                    box_numbers = box_numbers -> next;
                }

                // inserts random number from box_numbers list into grid
                grid[row][col] = box_numbers -> data;

                // update pointers in box_numbers
                if(box_num_remaining>1) {
                    temp = (node*) malloc(sizeof(node));

                    temp = box_numbers;
                    temp -> prev -> next = temp -> next;
                    temp -> next -> prev = temp -> prev;
                    box_numbers = temp -> next;
                }

                box_num_remaining--;
                col++;
            }
            row++;
        }
        free(temp);
        free(box_numbers);
    }

}

// iterates through grid to print game board
void display_board(int** grid) {
    int row;
    int col;

    // indents top left corner of board (header row)
    printf("    ");

    // prints column headers
    for(int c=1; c<=SIZE; c++) {
        // converts int to char
        printf("%c ", c+64);

        // increases spacing for new box headers, checks if end of a box horizontally
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
                // increases readability rather than using 0's
                printf("_ ");
            }else{
                printf("%d ", value);
            }

            // box separator, checks if end of a box horizontally
            if((col+1)%BOX_COLS == 0) {
                printf("| ");
            }
        }

        printf("\n");

        // inserts separator row between boxes, checks if end of box vertically
        if((row+1)%BOX_ROWS == 0) {
            sepearator_row();
        }
    }
}

// initialise grid
void init(int** grid) {
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
int get_box_num(int** grid, int row, int col) {
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
